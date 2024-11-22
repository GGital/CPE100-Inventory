# Git Branching Strategy Guide

This repository follows a simple branching strategy to maintain clean and organized code development. The process ensures that each new feature is developed in its own branch and merged into the `main` branch only after completion and testing.

---

## Workflow

### 1. Clone the Repository
Before starting, clone the repository to your local machine:  
```bash
git clone <repository-url>
cd <repository-name>
```

### 2. Create a New Branch
For each feature or task, create a new branch from the `main` branch. Use a meaningful name for the branch to describe the feature.  
```bash
git checkout main
git pull origin main  # Ensure your local main branch is up to date
git checkout -b feature/<feature-name>
```
**Example:**  
```bash
git checkout -b feature/user-login
```

### 3. Work on Your Feature
Make changes and commit them regularly to your feature branch.  
```bash
git add .
git commit -m "Describe your changes briefly"
```

### 4. Push the Branch to Remote
Push your feature branch to the remote repository for collaboration or backup.  
```bash
git push origin feature/<feature-name>
```

### 5. Create a Pull Request (PR)
Once the feature is complete and tested:
1. Navigate to your repository on GitHub.
2. Open a pull request from `feature/<feature-name>` to `main`.
3. Include a clear description of the feature and any testing information.
4. Assign reviewers (if applicable).

### 6. Review and Merge
After review (and approval, if applicable):
- Merge the pull request into the `main` branch.
- Delete the feature branch both locally and remotely.

### 7. Delete the Branch Locally
After merging the feature branch, delete it from your local repository to keep it clean.  
```bash
git branch -d feature/<feature-name>
```

### 8. Sync with Main
Always ensure your local `main` branch is up to date:  
```bash
git checkout main
git pull origin main
```

---

## Best Practices
- **Descriptive Branch Names:** Use clear, descriptive names for feature branches, e.g., `feature/user-authentication`.
- **Small Commits:** Make small, meaningful commits with descriptive messages.
- **Test Thoroughly:** Test your feature locally before creating a pull request.
- **Avoid Direct Commits to Main:** All changes should go through a feature branch.

---

## Common Git Commands

| Command                              | Description                           |
|--------------------------------------|---------------------------------------|
| `git checkout -b feature/<name>`     | Create a new feature branch           |
| `git add <file>`                     | Stage changes for commit              |
| `git commit -m "message"`            | Commit staged changes with a message  |
| `git push origin feature/<name>`     | Push feature branch to remote         |
| `git branch -d feature/<name>`       | Delete a local branch after merging   |
| `git pull origin main`               | Update local main branch              |

---

## Example Workflow
```bash
# Create a new feature branch
git checkout main
git pull origin main
git checkout -b feature/add-user-profile

# Work on the feature and commit changes
git add .
git commit -m "Add profile page UI"

# Push the branch to remote
git push origin feature/add-user-profile

# After review, merge via GitHub, then:
git checkout main
git pull origin main

# Delete the feature branch locally
git branch -d feature/add-user-profile
```

---

This branching strategy ensures a smooth development process, minimizes conflicts, and keeps the `main` branch production-ready.

--- 

Let me know if you'd like to make adjustments or add any specific details!
