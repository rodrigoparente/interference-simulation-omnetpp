#!/bin/bash

echo "Copying pathloss files..."
cp inet/physicallayer/pathloss/Indoor* ../inet/src/inet/physicallayer/pathloss 

echo "Copying network files..."
cp network.ned omnetpp.ini ../interference


