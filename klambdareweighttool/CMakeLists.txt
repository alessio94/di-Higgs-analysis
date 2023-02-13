################################################################################
# Package: hhTruthWeightTools
################################################################################

# Declare the package name:
atlas_subdir( hhTruthWeightTools )

# Declare the package's dependencies:
atlas_depends_on_subdirs( PUBLIC
                          Tools/PathResolver
                          Control/AthToolSupport/AsgTools
                          Event/xAOD/xAODParticleEvent
)

# Find the needed external(s):
find_package( ROOT COMPONENTS Core RIO Hist Tree )

# build a CINT dictionary for the library
atlas_add_root_dictionary ( hhTruthWeightToolsLib hhTruthWeightToolsCintDict
                            ROOT_HEADERS Root/LinkDef.h
                            EXTERNAL_PACKAGES ROOT
)

# Component(s) in the package:
atlas_add_library( hhTruthWeightToolsLib
                   hhTruthWeightTools/*.h Root/*.cxx ${hhTruthWeightToolsCintDict}
                   PUBLIC_HEADERS hhTruthWeightTools
                   LINK_LIBRARIES AsgTools PathResolver xAODParticleEvent
)

# Install files from the package:
atlas_install_data( data/* )
