#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "OpticalEquipment.hpp"
#include "Pattern.hpp"

class DLPPattern;
class GeneratedPattern;

/*
 * Class: Projector
 * Contains all information relevant to a physical projector, whether it is a
 * DLP, LED-and-grating, or any other kind of system. This system type is
 * defined by the <type> property and is used to make sure a given projector can
 * project a given pattern in <projectPattern>.
 *
 * Subclass of <OpticalEquipment>
 * Superclass of <DLPProjector>
 *
 * Properties:
 * type - the type of this projector, as defined in <ProjectorType> enum
 */
class Projector : OpticalEquipment {
};

class DLPProjector : Projector {
  public:
    /*
     * Method: projectPattern
     * Takes a <DLPProjector> and a <DLPPattern>, and projects the given
     * pattern with the given projector. It returns a <GeneratedPattern>
     * object representing the actual light field created by the projection.
     *
     * Simultaneously, it projects this pattern in the physical world using te
     * provided projector.
     *
     * Parameters:
     * projector - the dlp projector to be used to create the pattern
     * pattern - the definition of the pattern to be created by the projector
     *
     * Return:
     * a generated pattern that (nearly) perfectly represents the light field
     * created by the projector.
     */
    static GeneratedPattern projectPattern( const DLPProjector& projector, const DLPPattern& pattern);
};

#endif
