# ACCSharedMemory

ACCSharedMemory is a project containing C++ code that uses pybind11 in order to make a python package.
The main goal of the package is to directly accessing telemetry data from the shared memory of the game 
Assetto Corsa Competzione (ACC). This live data can then be used further for anything the user wants like 
for example a user interface which directly shows all your live actions while on the racetrack.


### Prerequisites
- C++ 17 windows 10 development environment
- Cmake 3.16+
- Python 3.9
- [pybind11](https://pybind11.readthedocs.io/en/stable/basics.html)
- Python venv with requirements.txt packages installed

### Setup
To quickly install the ACCSharedMemory module put the `/dist/ACCSharedMemory-0.1-py3.9-win-amd64.egg` package in your project and run the following command:
    
    python setup.py install

Ofcourse it is also possible to build the project yourself if you would like to make some changes to the code.
To build the project the `CmakeList.txt` can be used together with the `setup.py`. To install the newly built package just
run the above install command.

### Testing
After installing the packages can be tested by importing it your python file.

```python
import ACCSharedMemory as acc
    
#initialize sharedmemory
acc.initPhysics()
acc.initGraphics()
acc.initStatic()
    
#retrieve ACC data
physics = acc.getPhysicsData()
graphics = acc.getGraphicsData()
static = acc.getStaticData()
    
#Display data or perform other actions with dictionary
print(physics)
```