<map version="0.8.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1201691014537" ID="Freemind_Link_1198180720" MODIFIED="1201691034755" TEXT="Testing AnnotatedAStar">
<node CREATED="1201691091156" ID="_" MODIFIED="1201691273712" POSITION="right" STYLE="fork" TEXT="evaluate function" VSHIFT="-57">
<edge WIDTH="thin"/>
<node CREATED="1201692070538" ID="Freemind_Link_1451672214" MODIFIED="1201692118331" TEXT="no unique tests for cardinal or diagonal moves">
<node CREATED="1202289636420" ID="Freemind_Link_49616467" MODIFIED="1202289638665" TEXT="added!"/>
</node>
<node CREATED="1201691363563" FOLDED="true" ID="Freemind_Link_1275800872" MODIFIED="1201691385740" TEXT="Diagonal moves are hard to test">
<node CREATED="1201691436831" ID="Freemind_Link_241455122" MODIFIED="1201691462435" TEXT="needs to be equivalent to 2 cardinal steps"/>
<node CREATED="1201691392935" ID="Freemind_Link_751869755" MODIFIED="1201691413663" TEXT="recursive calls to other nodes">
<node CREATED="1201691466793" ID="Freemind_Link_1805326273" MODIFIED="1201694629187" TEXT="relies on presence of other (adjacent) nodes not part of TestExperiment data"/>
<node CREATED="1201691554401" ID="Freemind_Link_664679100" MODIFIED="1201693705278" TEXT="retrieved via (x,y) coordinates from HOG&apos;s mapAbstraction class"/>
<node CREATED="1201691618609" ID="Freemind_Link_670848603" MODIFIED="1201693757591" TEXT="mapAbstraction class not being tested&#xa;(assume HOG works)">
<node CREATED="1201693315520" ID="Freemind_Link_1138420911" MODIFIED="1201693336447" TEXT="no mock mapAbstraction object"/>
<node CREATED="1201693236247" ID="Freemind_Link_519903823" MODIFIED="1201693291729" TEXT="no easy way to pre-load nodes rigged with test-data"/>
</node>
<node CREATED="1201692543088" ID="Freemind_Link_1463769882" MODIFIED="1201692700334" TEXT="load a test map">
<icon BUILTIN="idea"/>
<node CREATED="1201692725466" ID="Freemind_Link_635286441" MODIFIED="1201692987437" TEXT="have a source for other nodes required by test">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1201692258142" ID="Freemind_Link_1431310715" MODIFIED="1201694122822" TEXT="AMA does not annotate the test map &#xa;(tests fail)">
<icon BUILTIN="button_cancel"/>
<node CREATED="1201692423189" ID="Freemind_Link_1757958682" MODIFIED="1201692441126" TEXT="no test code to load clearance value info"/>
<node CREATED="1201692379605" ID="Freemind_Link_1278968501" MODIFIED="1201692402472" TEXT="need to generate clearances manually"/>
<node CREATED="1201692407266" ID="Freemind_Link_156860375" MODIFIED="1201692420164" TEXT="need to load clearance values with map"/>
</node>
<node CREATED="1201693808623" ID="Freemind_Link_1240989859" MODIFIED="1201693857518" TEXT="reqts not well understood. could take ages and not be suitable">
<icon BUILTIN="button_cancel"/>
</node>
</node>
<node CREATED="1201693371591" ID="Freemind_Link_1350777334" MODIFIED="1201694313676" TEXT="hide mapAbstraction methods in AnnotatedMapAbstractionMock">
<icon BUILTIN="idea"/>
<node CREATED="1202278810849" ID="Freemind_Link_620778851" MODIFIED="1202278841862" TEXT="target: getNodeFromMap"/>
<node CREATED="1201693400285" ID="Freemind_Link_96722574" MODIFIED="1201693529467" TEXT="can insert nodes with rigged test data">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1201693613021" ID="Freemind_Link_1312319959" MODIFIED="1201693627546" TEXT="easy to implement">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1201693631861" ID="Freemind_Link_199320042" MODIFIED="1201694528861" TEXT="might need to code &quot;load testmap&quot; stuff later">
<icon BUILTIN="button_cancel"/>
</node>
<node CREATED="1201694316235" ID="Freemind_Link_191067929" MODIFIED="1201694374554" TEXT="need to create extra nodes manually in each test">
<icon BUILTIN="button_cancel"/>
</node>
<node CREATED="1201694458275" ID="Freemind_Link_227877134" MODIFIED="1201694569979" TEXT="need extra code to store/add nodes&#xa;(maybe a series; returned in order)"/>
</node>
<node CREATED="1202289500898" ID="Freemind_Link_1662864255" MODIFIED="1202289616689" TEXT="annotate nodes on demand based on current experiment">
<icon BUILTIN="idea"/>
<node CREATED="1202289522989" ID="Freemind_Link_1340582306" MODIFIED="1202289596517" TEXT="simple">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1202289532626" ID="Freemind_Link_717284636" MODIFIED="1202289596524" TEXT="no fancy hiding">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1202289564045" ID="Freemind_Link_618092212" MODIFIED="1202289596522" TEXT="no need for extra nodes">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1202289569863" ID="Freemind_Link_1726174355" MODIFIED="1202289605781" TEXT="do need additional code for each experiment that requires annotations (might scale poorly)">
<icon BUILTIN="button_cancel"/>
</node>
</node>
</node>
<node CREATED="1201691485764" ID="Freemind_Link_636136284" MODIFIED="1201691531866" TEXT="experiment objects contain no annotation info">
<node CREATED="1201691853108" ID="Freemind_Link_23376289" MODIFIED="1201691875614" TEXT="annotate nodes before each test">
<icon BUILTIN="idea"/>
<node CREATED="1201691916414" ID="Freemind_Link_1284252174" MODIFIED="1201691972101" TEXT="fixes problem">
<icon BUILTIN="button_ok"/>
</node>
<node CREATED="1201691976858" ID="Freemind_Link_133345132" MODIFIED="1201692004656" TEXT="some code duplication">
<icon BUILTIN="button_cancel"/>
</node>
</node>
</node>
</node>
</node>
<node CREATED="1202807646809" ID="Freemind_Link_735925781" MODIFIED="1202807703550" POSITION="left" TEXT="getPath function">
<node CREATED="1202807704048" ID="Freemind_Link_1507365629" MODIFIED="1202807865474" TEXT="valid &amp; invalid params">
<node CREATED="1202807729671" ID="Freemind_Link_250449299" MODIFIED="1202807736709" TEXT="size"/>
<node CREATED="1202807737497" ID="Freemind_Link_1807139374" MODIFIED="1202807738677" TEXT="caps"/>
<node CREATED="1202807739636" ID="Freemind_Link_187548725" MODIFIED="1202807748174" TEXT="map abstraction"/>
<node CREATED="1202807749272" ID="Freemind_Link_683472030" MODIFIED="1202807761665" TEXT="start node"/>
<node CREATED="1202807754981" ID="Freemind_Link_1668848100" MODIFIED="1202807757471" TEXT="goal node"/>
</node>
<node CREATED="1202807778459" ID="Freemind_Link_1287785002" MODIFIED="1202807857536" TEXT="correct A* behaviour">
<node CREATED="1202807846938" FOLDED="true" ID="Freemind_Link_775395841" MODIFIED="1202807850570" TEXT="tests">
<node CREATED="1202807809202" ID="Freemind_Link_65030444" MODIFIED="1202807817530" TEXT="goal  blocked (hard obstacle)"/>
<node CREATED="1202807818273" ID="Freemind_Link_1652122434" MODIFIED="1202807823248" TEXT="goal blocked (soft obstacle)"/>
<node CREATED="1202807824226" ID="Freemind_Link_516867321" MODIFIED="1202807842331" TEXT="goal found"/>
</node>
<node CREATED="1202808045529" ID="Freemind_Link_1485930552" MODIFIED="1202808049659" TEXT="involves lots of nodes"/>
<node CREATED="1202807884335" ID="Freemind_Link_801438518" MODIFIED="1202807971987" TEXT="map not fully annotated">
<node CREATED="1202807925232" ID="Freemind_Link_1661152228" MODIFIED="1202807933819" TEXT="using mock AMA"/>
<node CREATED="1202807935089" ID="Freemind_Link_185302228" MODIFIED="1202807997080" TEXT="other annotations hand-crafted per test"/>
</node>
<node CREATED="1202808065582" ID="Freemind_Link_1825361214" MODIFIED="1202808109259" TEXT="Option #1: load all annotations from file">
<node CREATED="1202808110358" ID="Freemind_Link_638959587" MODIFIED="1202808116630" TEXT="need to hand-craft file"/>
<node CREATED="1202808119314" ID="Freemind_Link_1031777150" MODIFIED="1202808150425" TEXT="hand-crafting error prone"/>
<node CREATED="1202808155411" ID="Freemind_Link_1982925297" MODIFIED="1202808185292" TEXT="need extra scaffolding code to load file"/>
<node CREATED="1202808187710" ID="Freemind_Link_1034302668" MODIFIED="1202808217231" TEXT="allows pure unit testing of A*"/>
</node>
</node>
</node>
</node>
</map>
