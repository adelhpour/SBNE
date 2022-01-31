from setuptools import setup
from setuptools import find_packages
from setuptools.dist import Distribution
import platform

import os.path, sys
if not os.path.isfile('libsbne.py'):
      print("First build the libsbne and then run this script.")
      sys.exit(1)

class BinaryDistribution(Distribution):

    def has_ext_modules(foo):
        return True


deps = []
if platform.system() == "Windows":
    deps = ['*.dll', '*.pyd', 'config6']
elif platform.system() == "Darwin" or platform.system() == "Linux":
    deps = ['*.so', '*.dylib', './dependencies/*', './dependencies/graphviz/*']

setup(
    name = "libsbne",
    version = "${SBNE_DOTTED_VERSION}",
    author = "Adel Heydarabadipour",
    author_email = "ad.heydarabadi@gmail.com",
    description = "LibSBNE Python API",
    long_description_content_type = "text/x-rst",
    long_description = ("LibSBNE is a library for systems/synthetic biologist to work with the Layout and Render extensions of SBML models. It enables the users to straightforwardly read, manipulate, and write and also automatically generate the information about the Layout and Render extensions of an SBML model."),
    url = "https://github.com/adelhpour/sbne",
    license = "MIT License",
    platforms = ["Windows", "MacOS", "Unix"],
    classifiers = [
        "Intended Audience :: Science/Research",
        "Programming Language :: C++",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: Unix"
    ],
    keywords= ["SBML", "Layout", "Render"],
    package_dir={"": "."},
    packages=find_packages(),
    package_data={"": deps},
    include_package_data = True,
    python_requires=">=3.7",
    distclass=BinaryDistribution
)
