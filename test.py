import ACCSharedMemory as acc
import pprint
from time import sleep
if __name__ == "__main__":
    acc.initPhysics()
    acc.initGraphics()
    acc.initStatic()


    while(True):
        physics = acc.getPhysicsData()
        graphics = acc.getGraphicsData()
        static = acc.getStaticData()

        # print()
        # print(physics)
        print()
        print(graphics['penalty'])
        print(graphics['flag'])
        print()
        # print(static)
        # print()

        sleep(5)
