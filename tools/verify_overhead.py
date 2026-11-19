#!/usr/bin/env python3
"""Optional GCC/Linux assembly comparison; not a universal performance proof."""
import argparse
import json
import platform
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--compiler', default='g++')
parser.add_argument('--output', type=Path, default=ROOT / 'build' / 'overhead')
args = parser.parse_args()
args.output.mkdir(parents=True, exist_ok=True)
version = subprocess.check_output([args.compiler, '--version'], text=True).splitlines()[0]
macros = subprocess.check_output([args.compiler, '-dM', '-E', '-x', 'c++', '-'], input='', text=True)
if '__GNUC__' not in macros or '__clang__' in macros or platform.system() != 'Linux':
    raise SystemExit('This automated parser is for GCC on Linux. See docs/PERFORMANCE.md for manual comparison.')
assembly = args.output / 'overhead.s'
flags = ['-std=c++20', '-O2', '-S', '-fno-ipa-icf']
subprocess.run([args.compiler, *flags, '-I', str(ROOT / 'include'),
                str(ROOT / 'benchmarks/overhead.cpp'), '-o', str(assembly)], check=True)
source = assembly.read_text()

def instructions(name):
    match = re.search(r'^' + re.escape(name) + r':\n(.*?)^\s*\.size\s+' +
                      re.escape(name) + r'\s*,', source, re.M | re.S)
    if not match:
        raise RuntimeError(f'Cannot find function body for {name}; inspect assembly manually.')
    result = []
    for line in match.group(1).splitlines():
        line = line.strip()
        if not line or line.startswith(('.', '#')) or line.endswith(':'):
            continue
        result.append(re.sub(r'\s+', ' ', line))
    if not result:
        raise RuntimeError(f'No instructions extracted for {name}')
    return result

results = []
for raw, typed in [('raw_speed', 'typed_speed'), ('raw_force', 'typed_force'),
                   ('raw_feet_to_metres', 'typed_feet_to_metres')]:
    raw_code, typed_code = instructions(raw), instructions(typed)
    results.append({'baseline': raw, 'typed': typed, 'identical': raw_code == typed_code,
                    'baseline_instructions': raw_code, 'typed_instructions': typed_code})
report = {'compiler': version, 'platform': platform.platform(), 'flags': flags,
          'comparison': 'Exact normalized instruction text; runtime parameters; three functions only',
          'results': results}
(args.output / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
for item in results:
    print(('MATCH' if item['identical'] else 'DIFFERENT'), item['baseline'], item['typed'])
print('Evidence:', args.output.resolve())
raise SystemExit(0 if all(item['identical'] for item in results) else 1)
