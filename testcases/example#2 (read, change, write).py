import libsbne as sbne

# set filename value to the absolute path of the SBML (.xml) file.
filename = "Absolute path to the sbml (.xml) file"

# set elementid value to the id of a network element.
elementid = "Id of a network element"

# set color value to a desired color for the element.
color = "Desired color for the element"

# create an infolist which is used by set function to set the stroke color of a network element
infolistset = {'filename': filename,
              'id': elementid,
              'stroke': color}

# create an infolist which is used by get function to get the stroke color of a network element
infolistget = {'filename': filename,
              'id': elementid,
              'key': 'stroke'}
              
# set the stroke color of the network element
if not sbne.ne_set(infolistset):
    print("The desired value is set")
else:
    print("Failed to set the desired value")

# get the stroke color of the network element
value = sbne.ne_get(infolistget)
if value:
    print("The desired value is: ", value)
else:
    print("Failed to get the desired value")
