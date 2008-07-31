<map version="0.8.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1202452497354" ID="Freemind_Link_973999882" MODIFIED="1202452666081" TEXT="DevNotes">
<node CREATED="1202452504179" FOLDED="true" ID="_" MODIFIED="1202452509435" POSITION="right" TEXT="annotations">
<node CREATED="1202452510063" ID="Freemind_Link_1103270848" MODIFIED="1202452527263" TEXT="3 valid terrains">
<node CREATED="1202452527869" ID="Freemind_Link_936970722" MODIFIED="1202452532728" TEXT="Ground"/>
<node CREATED="1202452534427" ID="Freemind_Link_809883622" MODIFIED="1202452539826" TEXT="Trees"/>
<node CREATED="1202452540615" ID="Freemind_Link_1413952293" MODIFIED="1202452543825" TEXT="Ground|Trees"/>
</node>
<node CREATED="1202452549881" ID="Freemind_Link_1965373593" MODIFIED="1202452560311" TEXT="Each node stores annotation for each valid terrain"/>
<node CREATED="1202452561587" ID="Freemind_Link_562713165" MODIFIED="1202452575506" TEXT="Each node stores a terrain type">
<node CREATED="1202452575903" ID="Freemind_Link_1790463340" MODIFIED="1202452585281" TEXT="can only set a valid terrain as the type"/>
<node CREATED="1202452587085" ID="Freemind_Link_1708686879" MODIFIED="1202452591448" TEXT="anything else is ignored"/>
</node>
<node CREATED="1202452594446" ID="Freemind_Link_1869694112" MODIFIED="1202452610446" TEXT="each node stores 3 clearance values -- one for each valid terrain">
<node CREATED="1202452610910" ID="Freemind_Link_1787656442" MODIFIED="1202452627578" TEXT="can only set a clearance value for a valid terrain type"/>
<node CREATED="1202452629063" ID="Freemind_Link_698898863" MODIFIED="1202452633983" TEXT="anything else is ignored"/>
</node>
</node>
<node CREATED="1203667263535" ID="Freemind_Link_856282637" MODIFIED="1203667344042" POSITION="left" TEXT="ACA">
<node CREATED="1203667300696" ID="Freemind_Link_1213936069" MODIFIED="1203667363535" TEXT="need a searchalg???"/>
<node CREATED="1203667320134" ID="Freemind_Link_586382509" MODIFIED="1203667325331" TEXT="::pathable">
<node CREATED="1203667276024" ID="Freemind_Link_197796552" MODIFIED="1203667281344" TEXT="does this need to be efficient?"/>
<node CREATED="1203667282444" ID="Freemind_Link_1391538393" MODIFIED="1203667299448" TEXT="look for a solution similar to HPA?"/>
<node CREATED="1203667374061" ID="Freemind_Link_1465698183" MODIFIED="1203667379101" TEXT="is it even required? ">
<node CREATED="1203667379729" ID="Freemind_Link_1356467283" MODIFIED="1203667383919" TEXT="original didn&apos;t use it"/>
</node>
</node>
</node>
<node CREATED="1203667316671" ID="Freemind_Link_1952351755" MODIFIED="1203667318273" POSITION="right" TEXT="ACA"/>
<node CREATED="1204434840368" ID="Freemind_Link_12124424" MODIFIED="1204434848012" POSITION="left" TEXT="AnnotatedCluster">
<node CREATED="1204434848425" ID="Freemind_Link_313056225" MODIFIED="1204434860123" TEXT="extending Cluster">
<node CREATED="1204434860759" ID="Freemind_Link_996965508" MODIFIED="1204434881313" TEXT="addNode uses nodeid as sole param"/>
<node CREATED="1204434883442" ID="Freemind_Link_702231528" MODIFIED="1204434905786" TEXT="AC needs to check node annotations before adding node to cluster"/>
<node CREATED="1204434906793" ID="Freemind_Link_1249695869" MODIFIED="1204434918036" TEXT="Q: do we change base method??"/>
<node CREATED="1204434922498" ID="Freemind_Link_299682658" MODIFIED="1204434946663" TEXT="Alternative: overload addNode (but original still available and now unsafe)"/>
<node CREATED="1204434950178" ID="Freemind_Link_568516253" MODIFIED="1204434970564" TEXT="Alternative #2: store reference to mapAbstraction in each cluster">
<node CREATED="1204434971131" ID="Freemind_Link_492633179" MODIFIED="1204434981029" TEXT="unclear where deletion responsibility lies"/>
<node CREATED="1204434982167" ID="Freemind_Link_1953748901" MODIFIED="1204435008259" TEXT="bad OO? (storing unnecessary stuff)"/>
</node>
</node>
<node CREATED="1204441185738" ID="Freemind_Link_1998488050" MODIFIED="1204441193716" TEXT="reassign nodes">
<node CREATED="1204441194164" ID="Freemind_Link_1117710508" MODIFIED="1204441209116" TEXT="do we really need to maximise cluster size?"/>
</node>
</node>
</node>
</map>
