# Scenario files #
A scenario file is a set of pre-generated experiments. Each scenario is specific one particular map and usually includes a number of experiments. A large number of scenario files were generated for evaluating the performance of HAA`*`; these are available in the `experiments/scenarios` subdirectory of the source tree. By default, each HAA`*` scenario contains 100 valid experiments (valid in the sense that each problem has a solution) that have been randomly generated.

The structure of a typical scenario file has 2 parts:
  * The first line describes the version number of the scenario file. For HAA`*` this is always 2.
  * The remaining lines describe specific experiments.

A typical entry for an experiment looks like this:

`maps/bgmaps/softobs_20pct/newAR0315SR.map       18      28      34      31      68      2       17`

  * The first column refers to the map used during the scenario.
  * The second and third columns refer to the `(x, y)` coordinates of the start position.
  * The third and fourth columns refer to the `(x, y)` coordinates of the goal position.
  * The fifth column refers to the terrain traversal capability of the agent in this experiment.
  * The sixth column refers to the size of the agent.

## Terrain Types ##
By default, all scenario files are run on maps with 2 terrains. Ground terrain (represented as `4`) and Trees terrain (represented as `64`). An agent's capability can include more than one terrain which is represented as the bitwise disjunction of a set of terrain types (thus, the Ground _or_ Trees capability is represented as `68`).

## Agent Sizes ##
All HAA`*` agents are assumed to be square in shape. All scenario files refer to agents of size `1` (occupying one map tile) or `2` (occupying four map tiles).