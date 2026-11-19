# Submission preparation

The project screenshot asks for a public C++ repository, documentation, a one-minute
demonstration and evidence of teamwork. Package-manager publication is optional.
This ZIP supplies source and documentation; it does not create a public repository.

## Fill in before submitting

| Required item | Team entry |
|---|---|
| Public repository URL | To fill |
| Public documentation URL | To fill |
| Actual team names and Git account names | To fill |
| Actual division of work | To fill using real contributions |
| Tested macOS/Windows/Linux compiler versions | To fill with actual runs |
| Reusable feature and comparison evidence | Update RELATED_WORK.md with measured findings |
| Supplementary evidence | Test output, demonstration and recorded assembly comparison |

## Repository setup example

Create an empty public repository on your Git hosting service. Then, in this project:

```sh
git init -b main
git add .
git commit -m "Add DimenGuard baseline library, examples and tests"
git remote add origin YOUR_REPOSITORY_URL
git push -u origin main
```

Use your own configured Git identity. Replace `YOUR_REPOSITORY_URL` with the real
repository URL. Team members should make subsequent commits under their own accounts.

## Rubric mapping

| Criterion | Supplied material | Still requires team action |
|---|---|---|
| Published | Ready-to-publish source, MIT license, ignore rules | Public repo and working link |
| Documented | English README, Chinese quick start, API/design/testing docs | Verify public accessibility and final facts |
| Functional | Working local Linux demo and automated checks | Demonstrate the actual target machine in class |
| Innovative | Reusable core and educational explanation API | Establish a meaningful distinction; do not claim an unproven gap |
| Teamwork | Proposed five-person ownership plan | Real contributions, history and joint presentation |

The grading criterion for innovation is stronger than simply reimplementing an
existing technique. Do not state that mp-units/Boost.Units cannot perform dimensional
analysis or use C++20 concepts. See the documented comparison and its limits.
