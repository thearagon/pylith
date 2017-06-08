#!/usr/bin/env nemesis
#
# ----------------------------------------------------------------------
#
# Brad T. Aagaard, U.S. Geological Survey
# Charles A. Williams, GNS Science
# Matthew G. Knepley, University of Chicago
#
# This code was developed as part of the Computational Infrastructure
# for Geodynamics (http://geodynamics.org).
#
# Copyright (c) 2010-2017 University of California, Davis
#
# See COPYING for license information.
#
# ----------------------------------------------------------------------
#
# Generate CUBIT/Trelis journal file to create slab surface from Slab
# 1.0 contours.
#
# We decimate the contours from Slab 1.0 in order to reduce the
# complexity of the slab geometry so that the mesh generation is
# faster.
#
# The Slab contours can be downloaded from:
# https://earthquake.usgs.gov/data/slab/models.php
#
# The reference for the Slab 1.0 model is:
#
# Hayes, G. P., D. J. Wald, and R. L. Johnson (2012), Slab1.0: A
# three-dimensional model of global subduction zone geometries,
# J. Geophys. Res., 117, B01302, doi:10.1029/2011JB008524.

# ----------------------------------------------------------------------
from pyre.applications.Script import Script as Application
from pyre.components.Component import Component
import pyre.inventory

import numpy

# ----------------------------------------------------------------------
class JournalFile(Component):
    """JournalFile object for writing CUBIT/Trelis journal file.
    """

    class Inventory(Component.Inventory):
        """Pyre properties and facilities for JournalFile.
        """
        filename = pyre.inventory.str("filename", default="geometry_surfs.jou")
        filename.meta["tip"] = "Name of generated CUBIT/Trelis journal file."
        
    
    def __init__(self, name="journalfile"):
        """Constructor.
        """
        Component.__init__(self, name=name, facility="journalfile")
        self.file = None
        return
    
    
    def writeHeader(self):
        """Write header for journal file.
        """
        if not self.file:
            self.file = open(self.filename, "w")
        self.file.write("# CUBIT/Trelis journal file generated by generate_slabsurfs.py\n" \
                        "#\n" \
                        "# Create ACIS NURBS surfaces for top and bottom of slab.\n\n")
        return

    def newSurface(self):
        """
        """
        self.file.write("# New surface.\n" \
                        "reset\n\n")
        return

    
    def skinSurface(self, filename):
        """Create surface and save it in an ACIS file.
        """
        self.file.write("# Create surface from curves.\n" \
                        "create surface skin curve all\n" \
                        "delete curve all\n\n" \
                        "# Save surface to ACIS file for later use.\n" \
                        "export acis '%s' overwrite\n\n" % filename)
        return
    
    
    def addContour(self, points):
        """Add contour to journal file.
        """
        self.file.write("# Contour\n")
        self.file.write("create vertex x %12.6e y %12.6e z %12.6e\n" % tuple(points[0]))
        self.file.write("${pBegin=Id('vertex')}\n")
        for pt in points[1:]:
            self.file.write("create vertex x %12.6e y %12.6e z %12.6e\n" % tuple(pt))            
        self.file.write("${pEnd=Id('vertex')}\n"\
                        "create curve spline vertex {pBegin} to {pEnd} delete\n\n")
        return


    def close(self):
        self.file.close()
        self.file = None
        return
    
    
# ----------------------------------------------------------------------
class SlabContoursFile(Component):
    """SlabContourFile object for reading contours from a Slab 1.0 contour file..
    """

    class Inventory(Component.Inventory):
        """Pyre properties and facilities for SlabContourFile.
        """
        filename = pyre.inventory.str("filename", default="cas_contours_dep.in.txt.gz")
        filename.meta["tip"] = "Name of ASCII file with slab contours."
        
    
    def __init__(self, name="slabcontourfile"):
        Component.__init__(self, name=name, facility="slabcontourfile")
        return
    
    
    def read(self):
        """Read contours from Slab 1.0 file.
        """

        if self.filename.endswith(".gz"):
            import gzip
            with gzip.open(self.filename, "rb") as file:
                lines = file.readlines()
        else:
            with open(self.filename, "r") as file:
                lines = file.readlines()
            
        contours = {}
        points = []
        key = None
        for line in lines:
            if line.strip() == "END":
                contours[key] = numpy.array(points, dtype=numpy.float64)
                points = []
                continue
            if len(line.split()) == 1:
                key = int(line)
                continue
            pt = map(float, line.strip().split())
            points.append(pt)
        self.contours = contours
        return

    
# ----------------------------------------------------------------------
class SlabExtender(Component):

    class Inventory(Component.Inventory):
        """Pyre properties and facilities for SlabExtender.
        """
        from pyre.units.length import km
        from pyre.units.angle import deg
        
        upDipElev = pyre.inventory.dimensional("up_dip_elev", default=1,0*km)
        upDipElev.meta["tip"] = "Elevation of contours extended in up-dip direction."
        
        upDipDist = pyre.inventory.dimensional("up_dip_dist", default=600,0*km)
        upDipDist.meta["tip"] = "Distance to extend contours in up-dip direction."
        
        upDipAngle = pyre.inventory.dimensional("up_dip_angle", default=10.0*deg)
        upDipAngle.meta["tip"] = "Distance to extend contours in up-dip direction."
        
        faultStrike = pyre.inventory.dimensional("fault_strike", default=0.0*deg)
        faultString.meta["tip"] = "Approximate strike of fault."

        contourStride = pyre.inventory.int("contour_stride", default=4)
        contourStride.meta["tip"] = "Stride to use in decimating number of contours."

        pointsStride = pyre.inventory.int("points_stride", default=20)
        pointsStride.meta["tip"] = "Stride to use in decimating number of points in a contour."
        

    def __init__(self, name="slabextender"):
        """Constructor.
        """
        Component.__init__(self, name=name, facility="slabextender")
        return
    

    def initialize(self, slab):
        self.contours = slab.contours
        self._decimate(stride)
        self._toXYZ()
        
    
    def addUpDipContours(self, elevKm=1.0, strikeDeg=0.0, dipDeg=10.0, extendDistKm=600.0):
        """Add contours up-dip from original contours.
        
        Keyword arguments:
        elevKm -- Elevation in kilometers of additional contours.
        strikeDeg -- Approximate strike of fault in degrees (default 0.0).
        dipDeg -- Dip angle in degrees at up-dip edge of fault (default 10.0).
        extendDistKm -- Distance in kilometers to extend slab surface in up-dip direction.

        We increase the horizontal distance between the contours at a
        geometric rate. The first contour is at a distance of
        distHoriz, followed by 2*distHoriz, 4*distHoriz, etc.

        The horizontal distance of contour n from the original one is
        (2**(n+1)-1)) * distHoriz, n=0,1,2,...

        """
        import math
        key = min(self.contours.keys())
        contourTop = self.contours[key]
        zTop = contourTop[0][2]

        distHoriz = (elevKm*1.0e+3-zTop)/math.tan(dipDeg*math.pi/180.0)
        dx = -distHoriz*math.cos(strikeDeg*math.pi/180.0)
        dy = distHoriz*math.sin(strikeDeg*math.pi/180.0)

        contoursUpDip = {}
        numContours = int(math.ceil(math.log((extendDistKm*1.0e+3/distHoriz)+1)/math.log(2.0)))
        for i in xrange(numContours):
            contour = numpy.array(contourTop)
            contour[:,0] += (2**i)*dx
            contour[:,1] += (2**i)*dy
            contour[:,2] = elevKm*1.0e+3
            contoursUpDip[-i] = contour
        self.contoursUpDip = contoursUpDip
        return
    
    
    def getContours(self, spacingKm=10.0):
        keys = []
        for k in self.contours.keys():
            if 0 == k % spacingKm:
                keys.append(k)
        contours = [self.contours[k] for k in sorted(keys)]
        return contours


    def getUpDipContours(self):
        contoursUpDip = [self.contoursUpDip[k] for k in sorted(self.contoursUpDip.keys())]
        return contoursUpDip

    
    def getAllContours(self, spacingKm=10.0):
        contours = self.getUpDipContours()
        contours += self.getContours(spacingKm)
        return contours

    
    def _decimate(self, stride):
        """Decimate the number of points in a contour.
        """
        for key,points in self.contours.items():
            pointsD = points[::stride]
            if (len(points)-1) % stride:
                pointsD = numpy.vstack((pointsD, points[-1],))
            self.contours[key] = numpy.ascontiguousarray(pointsD)
        return 

    
    def _toXYZ(self):
        """Transform from geographic coordinates and depth in km to geographic
        projected coordinate system. The coordinate system is imported
        from coordsys.py.

        """
        import coordsys
        for points in self.contours.values():
            coordsys.geoToMesh(points)
            points[:,2] *= 1.0e+3
        return


# ----------------------------------------------------------------------
class SurfaceApp(Application):
    """SurfaceApp object for top-level application workflow.
    """

    class Inventory(Component.Inventory):
        """Pyre properties and facilities for SurfaceApp.
        """
        from pyre.units.length import km
        from pyre.units.angle import deg

        modeler = pyre.inventory.facility("cubit", factory=JournalFile)
        modeler.meta["tip"] = "Surface modeler for slab."

        contours = pyre.inventory.facility("slab", factory=SlabContoursFile)
        contours.meta["tip"] = "Slab contours."

        extender = pyre.inventory.facility("extender", factory=SlabExtender)
        extender.meta["tip"] = "Extender for slab."
        
        slabTopFilename = pyre.inventory.str("slab_top_filename", default="surf_slabtop.sat")
        slabTopFilename.meta["tip"] = "Name of ACIS file with slab top surface."
        
        slabBotFilename = pyre.inventory.str("slab_bot_filename", default="surf_slabbot.sat")
        slabBotFilename.meta["tip"] = "Name of ACIS file with slab bottom surface."
        
        splayFilename = pyre.inventory.str("splay_filename", default="surf_splay.sat")
        splayFilename.meta["tip"] = "Name of ACIS file with splay fault surface."

        slabNormalDir = pyre.inventory.list("slab_normal_dir", default=[0,0,1])
        slabNormalDir.meta["tip"] = "Approximate average upward normal direction for slab."
        
    
    def __init__(self, name="surfaceapp"):
        """Constructor.
        """
        Application.__init__(self, name)
        return

    
    def main(self):
        """Open Slab 1.0 input file and journal output file and loop over
        contours.
        """
        
        self.contours.read()

        self.extender.initialize(self.contours)
        self.extender.addUpDipContours()

        self.modeler.writeHeader()

        # Top of slab
        self.modeler.newSurface()
        for contour in self.extender.getAllContours():
            self.modeler.addContour(contour)
        self.modeler.skinSurface("surf_slabtop.sat")

        # Bottom of slab
        normalX = -0.209
        normalY = +0.016
        normalZ = -0.979
        slabThicknessKm = 50.0
        
        self.modeler.newSurface()
        for contour in self.extender.getUpDipContours():
            contour[:,2] = -slabThicknessKm*1.0e+3
            self.modeler.addContour(contour)
        
        for contour in self.extender.getContours():
            contour[:,0] += slabNormalDir[0]*slabThicknessKm*1.0e+3
            contour[:,1] += slabNormalDir[1]*slabThicknessKm*1.0e+3
            contour[:,2] += slabNormalDir[2]*slabThicknessKm*1.0e+3
            self.modeler.addContour(contour)
        self.modeler.skinSurface("surf_slabbot.sat")

        # Splay fault
        self.modeler.newSurface()
        contour = extender.contours[15]
        contour[:,2] -= 5.0e+3
        self.modeler.addContour(contour)
        contour[:,2] = 1.0e+3
        contour[:,0] -= 15.0e+3
        self.modeler.addContour(contour)
        self.modeler.skinSurface("surf_splay.sat")
        
        self.modeler.close()
        return


# ======================================================================
if __name__ == "__main__":

    from pyre.applications import start
    start(applicationClass=SurfaceApp)

    
# End of file
