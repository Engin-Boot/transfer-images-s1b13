# Objective of Module

This module keeps track of images and store them with Diagnosis status in a CSV file.

# Usage

-Install all dependencies(Python libraries).
-Run ImageTracker.py and pass path of images folder in command line arguments.
-Run ImageTrackerTest.py to run test cases.

# Description

Whenever Python program runs following steps take place:
- The Python program traverses the images folder and finds the images.
- The images are initially stored in CSV file with status as Diagnosis Pending.
- When a new image file is updated in images folder it also gets updated in CSV.
- The user can update the status(Diagnosis Complete/Diagnosis Pending) for the image files using Python application.
- The changes(Diagnosis status) done by user are also reflected in CSV file.
