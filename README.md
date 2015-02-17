###############
ISLESevaluation
###############

Evaluation script for the ISLES challenge 2015 (www.isles-challenge.org).

Background libarary
-------------------
Covalic: https://github.com/InsightSoftwareConsortium/covalic/

Requirements
------------
ITK

Compilation
-----------
cmake .
make

Usage
-----
./validateLabelImages-ISLES truth.nii[.gz] test.nii[.gz]
