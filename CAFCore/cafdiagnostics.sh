#!/bin/bash

current_path="$PWD"

make_package_diagnostic () {
  package_name="$1"
  package_dir="${CAFANALYSISBASE}/$2"
  cd "$package_dir"
  echo ===================
  echo Package: $package_name
  echo ===================
  echo $PWD
  echo
  echo Current branch: $(git branch | sed -n '/\* /s///p')
  echo Current commit: $(git rev-parse HEAD)
  echo
  echo Output of 'git status'
  git status
  echo
  echo Remotes:
  git remote -v
  echo
  echo
}

date
make_package_diagnostic CAFCore CAFCore
make_package_diagnostic CAFExample

cd "$current_path"
