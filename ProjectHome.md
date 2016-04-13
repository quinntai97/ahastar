# Motivation #
You might think path planning is a solved problem but you'd be wrong.  The classical A`*` algorithm is proven optimal but can be very slow on large maps. Existing hierarchical planners (like HPA`*` and PRA`*`) can help in these situations but they are not able to deal with multi-size agents. More recent planners like TRA`*` and the Corridor Map Method can but neither is able to deal with really interesting environments that feature lots of terrains and other complex topographical features.

Hierarchical Annotated A`*` is our attempt to address these challenges. HAA`*` is a hierarchical path planner that uses a simple fixed-size gridworld approach. It combines clearance-based planning with a cluster-based abstraction ala HPA`*` and allows multi-size agents to efficiently plan high quality paths in heterogenous-terrain environments.

# Overview #
HAA`*` is a pathfinding algorithm for multi-size agents in heterogenous-terrain gridworld environments. It features:
  * Efficient hierarhical path planning approach (extends HPA`*`).
  * Supports agents of different sizes and with different terrain traversal capabilities.
  * Information rich yet compact state-space abstraction that reflects changing topographical features.
  * Built using [Hierarchical Open Graph](http://www.cs.ualberta.ca/~nathanst/hog.html) (HOG) pathfinding library

HAA`*` is also easily extended to include further support for other terrain features like elevation.

## Papers ##
Harabor D. & Botea, A. (2008), _Hierarchical path planning for multi-size agents in heterogeneous environments_, IEEE Symposium on Computational Intelligence and Games (CIG'08), Perth Australia [(pdf)](https://dl.getdropbox.com/u/361091/haastar/haa.pdf) [(presentation)](https://dl-web.getdropbox.com/get/presentations/haa_cig08.pdf)

**DRAFT** Harabor, D. & Botea, A. (2008),  _Hierarchical path planning for multi-size agents in heterogenous environments_, ICAPS Workshop on Knowledge Engineering for Planning and Scheduling (KEPS), Sydney Australia [(pdf)](http://ktiml.mff.cuni.cz/~bartak/KEPS2008/download/paper05.pdf)