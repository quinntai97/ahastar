# Installing #

On OSX/`*`nix machines, simply run `make` in the top-level folder (usually `ahastar`).
This will compile HOG and create a new application, `bin/sample` which will invoke the HOG gui.

## Unit Tests ##
There are a large number of unit tests to verify the correct operation of the HAA`*` algorithm. To run these tests you will need the [CPPUnit](http://sourceforge.net/projects/cppunit) framework and [MockPP](http://mockpp.sourceforge.net/) library. The instructions described here are specific to OSX (this is the primary development platform). Linux/BSD/Windows will probably require some extra work to point HOG to the right library paths once everything is installed.
On OSX, HOG is configured to link to a CPPUnit installation in `/opt/local` and a MockPP installation in `/usr/local`.

The easiest way to run the tests is via Xcode on OSX. Load the project via the pre-made hog.xcodeproj project file included in the top-level directory of the source distribution. First, compile the `hog` target (usually in Debug mode).

Next, update the `HOGHOME` variable found in `tests/TestConstants.h`. This should be an absolute path to the directory where the HOG source distribution is installed on the local machine. For example, on my mac I set `HOGHOME` to `/Users/dharabor/src/ahastar`. This step is important as the unit tests make use of test maps on which the algorithm is exercised; the location of these maps will depend on where you extracted the source.

Finally, compile and run the `testsimulation` target (again, usually in Debug mode).

# Launching HOG #

The `sample` binary takes several command line arguments:
```
-map : -map filename
     Selects the default map to be loaded. A large number of maps are available in the `maps` subdirectory.
-genscenarios : -genscenarios [.map filename] [number of scenarios] [clearance]
     Generates a scenario; a set of path problems on a given map
-scenario : -scenario filename
     Execute all experiments in a given .scenario file
-gui : -gui enable/disable
     Run the app without a pretty interface (used in conjunction with -scenario). Defaults to enable if not specified or if a non-valid argument is given 
-quality : -quality high/low
     Type of cluster abstraction to create (high results in better quality solutions but costs more memory & takes longer to set up). Default = high
-clustersize : -clustersize [num]
     Size of clusters to split up map into. Larger clusters are faster to create but less accurate. Default = 10.
```

By default, in GUI mode the map will be displayed and an overlay showing the position of HAA`*` clusters and entrances. Shortest paths between each pair of entrances inside a cluster are also overlaid. Currently, this overlay is hardcoded and cannot be turned off without modifying the `AnnotatedClusterAbstraction::openGLDraw` function.

An additional overlay, showing clearance values for various terrain types, can be enabled by modifying the `AnnotatedMapAbstraction::openGLDraw` function.

## Running experiments interactively ##
Single agents may be deployed interactively if HOG is launched in GUI mode.
The following keyboard commands are available:
```
a: Launch an A* agent with a randomly selected start and goal location.
Shift-a: Launch an HAA* agent with randomly selected start and goal location.
p: Pause/resume the simulation.
```

Alternatively, entire scenario files may be run interactively or without a GUI, depending on how HOG was launched.
Example usage:
```
bin/sample -scenario experiments/scenarios/softobs_20pct/newAR0315SR.scenario
```

By default, each scenario file is executed by two agents, one using the standard A`*` algorithm and the other using the HAA`*` algorithm.

## Experimental results ##
Things go here.