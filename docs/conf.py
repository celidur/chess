import sys
import os

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'chess'
copyright = '2023, Charles Khoury, Gaëtan Florio'
author = 'Charles Khoury, Gaëtan Florio'
release = '1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.todo', 'breathe', 'sphinx.ext.autodoc', 'm2r2']

exclude_patterns = ["venv", f"html/breathe/doxygen/{project}/xml/*.xml"]

source_suffix = ['.rst', '.md', '.xml']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# Breathe config
sys.path.append("./breathe/")
breathe_projects = {project: "./xml-dir/"}
breathe_show_include = False
breathe_default_project = project


def iterate_over_dirs(path: str, exclude_dirs: list[str], file_types: list[str], source_files: list[str] = None) \
        -> list[str]:
    if source_files is None:
        source_files = []
    for file in os.listdir(path):
        if file in exclude_dirs:
            continue
        possible_dir = os.path.join(path, file)
        if os.path.isdir(possible_dir):
            iterate_over_dirs(possible_dir, exclude_dirs, file_types, source_files)
        elif os.path.splitext(file)[1] in file_types:
            source_files.append(possible_dir)
    return source_files


# Add all source files to be generated and included by sphinx and breathe
project_dir = os.path.abspath(os.path.join("..", project))
project_exclude_dirs = ["cmake-build-debug"]
project_accepted_file_types = [".cpp", ".h"]
project_generated_files_folder = "code-doc"
project_files = iterate_over_dirs(project_dir, project_exclude_dirs, project_accepted_file_types)

breathe_projects_source = {project: (project_dir, project_files)}

if not os.path.exists(project_generated_files_folder):
    os.makedirs(project_generated_files_folder)

for source_file in project_files:
    source_path, source_file = os.path.split(source_file)
    source_file, _ = os.path.splitext(source_file)
    rst_file = f"{project_generated_files_folder}/{source_file.lower()}.rst"
    if not os.path.exists(rst_file):
        with open(rst_file, "w") as f:
            title = source_file
            f.write(f"{title}\n{'=' * len(title)}\n\n")

            for file_type in project_accepted_file_types:
                full_path = os.path.join(source_path, f"{source_file}{file_type}")
                if os.path.exists(full_path):
                    relative_path = os.path.relpath(full_path, project_dir)
                    f.write(f".. doxygenfile:: {relative_path}\n   :project: {project}\n")
