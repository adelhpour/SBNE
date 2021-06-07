from setuptools import setup
from setuptools import find_packages

import os.path, sys
if not os.path.isfile('libsbne.py'):
      print("First build the libsbne and then run this script.")
      sys.exit(1)

setup(
    name = "libsbne",
    version = "${SBNE_DOTTED_VERSION}",
    author = "Adel Heydarabadipour",
    author_email = "ad.heydarabadi@gmail.com",
    description = "a swig-wrapped python module of libSBNE (API for layout and render extensions of libSBML)",
    url = "https://github.com/adelhpour/sbne",
    classifiers = [
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    package_dir={"libsbne": "."},
    packages=find_packages(),
    package_data={"": ['*.dll', '*.so', '*.pyd', '*.dylib']},
    python_requires=">=3.6"
)
