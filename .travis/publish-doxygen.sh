#!/bin/bash -e

# Settings
REPO_PATH=git@github.com:pulsar-chem/Pulsar-Core.git
HTML_PATH=html
COMMIT_USER="Documentation Builder"
CHANGESET=$(git rev-parse --verify HEAD)

# Get a clean version of the HTML documentation repo.
mkdir -p ${HTML_PATH}
git clone -b gh-pages "${REPO_PATH}" --single-branch ${HTML_PATH}

# rm all the files through git to prevent stale files.
ls dox
ls pulsar
#uncomment once we get a successful build
#cd ${HTML_PATH}
#git rm -rf .
#cd -

# Generate the HTML documentation.
doxygen

# Create and commit the documentation repo.
cd ${HTML_PATH}
git add .
git config user.name "${COMMIT_USER}"
git config user.email "<>"
git commit -m "Automated documentation build for changeset ${CHANGESET}."
git push origin gh-pages
cd -
