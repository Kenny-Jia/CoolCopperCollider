# ==========================================================================
#  AIDA Detector description implementation
# --------------------------------------------------------------------------
# Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
# All rights reserved.
#
# For the licensing terms see $DD4hepINSTALL/LICENSE.
# For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
#
# ==========================================================================
#
"""

   Subtest using CLICSid showing the usage the HEPMC file reader

   @author  M.Frank
   @version 1.0

"""

from __future__ import absolute_import, unicode_literals

import sys
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def run(filename):
  import CLICSid
  import DDG4
  import os
  import sys
  import time
  from DDG4 import OutputLevel as Output

  sid = CLICSid.CLICSid(no_physics=False)
  geant4 = sid.geant4
  kernel = sid.kernel
  sid.loadGeometry()
  geant4.printDetectors()
  kernel.UI = 'UI'
  if len(sys.argv) >= 2 and sys.argv[1] == "batch":
    DDG4.setPrintLevel(DDG4.OutputLevel.WARNING)
    kernel.UI = ''
  geant4.setupCshUI()
  sid.setupField(quiet=False)
  DDG4.importConstants(kernel.detectorDescription(), debug=False)

  geant4.setupROOTOutput('ROOTOutput', 'C3SiD_' + filename + '_' + time.strftime('%Y-%m-%d_%H-%M'))

  prt = DDG4.EventAction(kernel, 'Geant4ParticlePrint/ParticlePrint')
  prt.OutputLevel = Output.INFO
  prt.OutputType = 3  # Print both: table and tree
  kernel.eventAction().adopt(prt)

  # First particle file reader
  gen = DDG4.GeneratorAction(kernel, "Geant4GeneratorActionInit/GenerationInit")
  kernel.generatorAction().adopt(gen)
  input = DDG4.GeneratorAction(kernel, "Geant4InputAction/Input")
  fname = "./" + filename + ".txt" 
  input.Input = "Geant4EventReaderMUCARLO|" + fname
  input.MomentumScale = 1.0
  input.Mask = 1
  kernel.generatorAction().adopt(input)

  # Merge all existing interaction records
  merger = DDG4.GeneratorAction(kernel, "Geant4InteractionMerger/InteractionMerger")
  merger.enableUI()
  kernel.generatorAction().adopt(merger)

  logger.info("#  Finally generate Geant4 primaries")
  gen = DDG4.GeneratorAction(kernel, "Geant4PrimaryHandler/PrimaryHandler")
  gen.OutputLevel = 4  # generator_output_level
  gen.enableUI()
  kernel.generatorAction().adopt(gen)

  # And handle the simulation particles.
  part = DDG4.GeneratorAction(kernel, "Geant4ParticleHandler/ParticleHandler")
  kernel.generatorAction().adopt(part)
  part.OutputLevel = Output.INFO
  part.enableUI()

  logger.info("#  Configure Event actions")
  prt = DDG4.EventAction(kernel, 'Geant4ParticlePrint/ParticlePrint')
  prt.OutputLevel = Output.INFO
  prt.OutputType = 3  # Print both: table and tree
  kernel.eventAction().adopt(prt)

  user = DDG4.Action(kernel, "Geant4TCUserParticleHandler/UserParticleHandler")
  user.TrackingVolume_Zmax = DDG4.ECalEndcap_zmin
  user.TrackingVolume_Rmax = DDG4.ECalBarrel_rmin
  user.enableUI()
  part.adopt(user)
  #
  sid.setupDetectors()
  sid.setupPhysics('QGSP_BERT')

  rng = sid.setupRandom('R1', seed=76545678, type='MTwistEngine')
  DDG4.Geant4Random.setMainInstance(rng.get())


  sid.test_run(have_geo=True, num_events=1)



if __name__ == "__main__":
  if len(sys.argv) < 2:  # No filename passed
    print("Please provide a filename as a command-line argument.")
    sys.exit(1)
  run(sys.argv[1])
