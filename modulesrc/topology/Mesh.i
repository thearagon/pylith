// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2017 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

/**
 * @file modulesrc/topology/Mesh.hh
 *
 * @brief Python interface to C++ PyLith Mesh object.
 */

namespace pylith {
    namespace topology {
        // Mesh -------------------------------------------------------------
        class Mesh { // Mesh
                     // PUBLIC METHODS /////////////////////////////////////////////////
public:

            /// Default constructor.
            Mesh(void);

            /** Constructor with dimension and communicator.
             *
             * @param dim Dimension associated with mesh cells.
             * @param comm MPI communicator for mesh.
             */
            Mesh(const int dim,
                 const MPI_Comm& comm=PETSC_COMM_WORLD);

            /// Default destructor
            ~Mesh(void);

            /// Deallocate PETSc and local data structures.
            virtual
            void deallocate(void);

            /** Set coordinate system.
             *
             * @param cs Coordinate system.
             */
            void setCoordSys(const spatialdata::geocoords::CoordSys* cs);

            /** Get coordinate system.
             *
             * @returns Coordinate system.
             */
            const spatialdata::geocoords::CoordSys* getCoordSys(void) const;

            /** Set debug flag.
             *
             * @param value Turn on debugging if true.
             */
            void debug(const bool value);

            /** Get debug flag.
             *
             * @param Get debugging flag.
             */
            bool debug(void) const;

            /** Get dimension of mesh.
             *
             * @returns Dimension of mesh.
             */
            int dimension(void) const;

            /** Get the number of vertices per cell
             *
             * @returns Number of vertices per cell.
             */
            int numCorners(void) const;

            /** Get number of vertices in mesh.
             *
             * @returns Number of vertices in mesh.
             */
            int numVertices(void) const;

            /** Get number of cells in mesh.
             *
             * @returns Number of cells in mesh.
             */
            int numCells(void) const;

            /** Get MPI communicator associated with mesh.
             *
             * @returns MPI communicator.
             */
            const MPI_Comm comm(void) const;

            /** View mesh.
             *
             * @param viewOption PETSc DM view option.
             *
             * PETSc mesh view options include:
             *   short summary [empty]
             *   detail summary ::ascii_info_detail
             *   detail in a file :refined.mesh:ascii_info_detail
             *   latex in a file  :refined.tex:ascii_latex
             *   VTK vtk:refined.vtk:ascii_vtk
             */
            void view(const char* viewOption="") const;

        }; // Mesh

    } // topology
} // pylith

// End of file
