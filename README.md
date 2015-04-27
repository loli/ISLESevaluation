###############
ISLESevaluation
###############

Evaluation script for the ISLES challenge 2015 (www.isles-challenge.org).

Background libarary
-------------------
Covalic: https://github.com/InsightSoftwareConsortium/covalic/

Requirements
------------
ITK: http://www.itk.org/

Compilation
-----------
```
cmake .
make
```

Usage
-----
```
LINUX and MAC
./validateLabelImages-ISLES truth.nii[.gz] test.nii[.gz] evaluationresults.json (optional)

WINDOWS
validateLabelImages-ISLES.exe truth.nii[.gz] test.nii[.gz] evaluationresults.json (optional)

```
