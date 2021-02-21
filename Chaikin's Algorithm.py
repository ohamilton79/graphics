#Chaikin Curve Generator

import matplotlib.pyplot as plt

#Define control points
controlPoints = [
    (0.0, 0.0),
    (2.0, 0.0),
    (1.0, 1.0),
    (0.0, 1.0),
    (1.5, 2.0)
]

def getMidpoint(point1, point2):
    #Get the x and y components
    x1 = point1[0]
    y1 = point1[1]
    
    x2 = point2[0]
    y2 = point2[1]

    #Add them together
    totalX = x1 + x2
    totalY = y1 + y2

    #Divide by 2
    newPoint = (totalX/2, totalY/2)
    return newPoint

#Function to scale a point, passed as a tuple
def scalePoint(point1, point2, scalar):
    #Get the midpoint
    midpoint = getMidpoint(point1, point2)

    #If the scalar is 0.75, get the midpoint of the original midpoint and endpoint
    if scalar == 0.75:
        newPoint = getMidpoint(midpoint, point2)

    #If the scalar is 0.25, get the midpoint of the start point and original midpoint
    if scalar == 0.25:
        newPoint = getMidpoint(point1, midpoint)

    return newPoint

#Function to generate new control points (create a new control polygon)
def generateNewPoints(controlPoints):
    
    newControlPoints = controlPoints

    for x in range(6):
        tempControlPoints = []
        for index in range(len(newControlPoints)-1):
            #Get the start and endpoints
            startPoint = newControlPoints[index]
            endPoint = newControlPoints[index+1]

            #Generate new start and end points using the '1/4 3/4' algorithm
            
            newStartPoint = scalePoint(startPoint, endPoint, 0.25)
            newEndPoint = scalePoint(startPoint, endPoint, 0.75)

            #Add to the new control points list
            tempControlPoints.append(newStartPoint)
            tempControlPoints.append(newEndPoint)

        #Compare the last point with the first point
        startPoint = newControlPoints[len(newControlPoints)-1]
        endPoint = newControlPoints[0]
        #Generate new points
        newStartPoint = scalePoint(startPoint, endPoint, 0.25)
        newEndPoint = scalePoint(startPoint, endPoint, 0.75)

        #Add to the control points list
        tempControlPoints.append(newStartPoint)
        tempControlPoints.append(newEndPoint)

        #Assign the temp control points to the new control points list before the next iteration
        newControlPoints = tempControlPoints

    #Assemble x and y values
    print(newControlPoints)
    xValues = []
    yValues = []
    for point in newControlPoints:
        xValues.append(point[0])
        yValues.append(point[1])


    #Plot points
    #plt.scatter(xValues, yValues)
    plt.plot(xValues, yValues, color="red")
    #Plot a line from the first point to the last, to form a closed shape
    plt.plot([xValues[0], xValues[len(xValues)-1]], [yValues[0], yValues[len(yValues)-1]], color="red")
    #Show plot
    plt.show()

    

    
    
generateNewPoints(controlPoints)
