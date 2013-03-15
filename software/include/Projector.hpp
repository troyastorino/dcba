#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "OpticalEquipment.hpp"
#include "Pattern.hpp"

class Pattern;
class GeneratedPattern;

/*
 * Class: Projector
 * Contains all information relevant to a physical projector, whether it is a
 * DLP, LED-and-grating, or any other kind of system. This system type is
 * defined by the <type> property and is used to make sure a given projector can
 * project a given pattern in <projectPattern>.
 *
 * Subclass of OpticalEquipment
 *
 * Properties:
 * type - the type of this projector, as defined in <ProjectorType> enum
 */
class Projector : OpticalEquipment {
public:
  /*
   * Enumeration: ProjectorType
   *
   * An enumerated type that defines the available types of projectors. This is
   * used to specify the type of projector used to project each pattern.
   */
  enum ProjectorType {
    DLP,
    STATIC_LED
  };
  /*
   * Method: projectPattern
   * Takes a <Projector> and a <Pattern> and, if the given projector can project the
   * given pattern, returns a <GeneratedPattern> object that contains the matrix
   * of pixel values representing the light pattern created by the projector.
   * This takes into account the spacial location and orientation of the
   * projector and any distortions of the projector from the ideal.
   *
   * Simultaneously, it projects this pattern in the physical world using te
   * provided projector.
   *
   * Parameters:
   * projector - the projector to be used to create the pattern
   * pattern - the definition of the pattern to be created by the projector
   *
   * Return:
   * a generated pattern that (nearly) perfectly represents the light field
   * created by the projector.
   */
  static GeneratedPattern projectPattern(Projector& projector, Pattern& pattern);

protected:
  /*
   * Property: type
   * The type of the projector. This is used to check what kind of patterns can
   * be projected by this projector.
   */
  ProjectorType type;
};

#endif
