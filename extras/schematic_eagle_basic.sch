<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="3" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="15" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="15" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="101" name="Hidden" color="15" fill="1" visible="yes" active="yes"/>
<layer number="102" name="Changes" color="12" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="diy-modules">
<description>&lt;b&gt;DIY Modules for Arduino, Raspberry Pi, CubieBoard etc.&lt;/b&gt;
&lt;br&gt;&lt;br&gt;
The library contains a list of symbols and footprints for popular, cheap and easy-to-use electronic modules.&lt;br&gt;
The modules are intend to work with microprocessor-based platforms such as &lt;a href="http://arduino.cc"&gt;Arduino&lt;/a&gt;, &lt;a href="http://raspberrypi.org/"&gt;Raspberry Pi&lt;/a&gt;, &lt;a href="http://cubieboard.org/"&gt;CubieBoard&lt;/a&gt;, &lt;a href="http://beagleboard.org/"&gt;BeagleBone&lt;/a&gt; and many others. There are many manufacturers of the modules in the world. Almost all of them can be bought on &lt;a href="ebay.com"&gt;ebay.com&lt;/a&gt;.&lt;br&gt;
&lt;br&gt;
By using this library, you can design a PCB for devices created with usage of modules. Even if you do not need to create PCB design, you can also use the library to quickly document your work by drawing schematics of devices built by you.&lt;br&gt;
&lt;br&gt;
The latest version, examples, photos and much more can be found at: &lt;b&gt;&lt;a href="http://diymodules.org/eagle"&gt;diymodules.org/eagle&lt;/a&gt;&lt;/b&gt;&lt;br&gt;&lt;br&gt;
Comments, suggestions and bug reports please send to: &lt;b&gt;&lt;a href="mailto:eagle@diymodules.org"&gt;eagle@diymodules.org&lt;/b&gt;&lt;/a&gt;&lt;br&gt;&lt;br&gt;
&lt;i&gt;Version: 1.8.0 (2017-Jul-02)&lt;/i&gt;&lt;br&gt;
&lt;i&gt;Created by: Miroslaw Brudnowski&lt;/i&gt;&lt;br&gt;&lt;br&gt;
&lt;i&gt;Released under the Creative Commons Attribution 4.0 International License: &lt;a href="http://creativecommons.org/licenses/by/4.0"&gt;http://creativecommons.org/licenses/by/4.0&lt;/a&gt;&lt;/i&gt;
&lt;br&gt;&lt;br&gt;
&lt;center&gt;
&lt;a href="http://diymodules.org/eagle"&gt;&lt;img src="http://www.diymodules.org/img/diymodules-lbr-image.php?v=1.8.0" alt="DIYmodules.org"&gt;&lt;/a&gt;
&lt;/center&gt;</description>
<packages>
<package name="ARDUINO-PRO-MINI">
<description>&lt;b&gt;Arduino Pro Mini&lt;/b&gt;</description>
<pad name="J6.12" x="15.24" y="7.62" drill="1"/>
<pad name="J6.11" x="12.7" y="7.62" drill="1"/>
<pad name="J6.10" x="10.16" y="7.62" drill="1"/>
<pad name="J6.9" x="7.62" y="7.62" drill="1"/>
<pad name="J6.8" x="5.08" y="7.62" drill="1"/>
<pad name="J6.7" x="2.54" y="7.62" drill="1"/>
<pad name="J6.6" x="0" y="7.62" drill="1"/>
<pad name="J6.5" x="-2.54" y="7.62" drill="1"/>
<pad name="J6.4" x="-5.08" y="7.62" drill="1"/>
<pad name="J6.3" x="-7.62" y="7.62" drill="1"/>
<pad name="J6.2" x="-10.16" y="7.62" drill="1"/>
<pad name="J6.1" x="-12.7" y="7.62" drill="1"/>
<pad name="J3.3" x="15.24" y="5.08" drill="1"/>
<pad name="J3.2" x="15.24" y="2.54" drill="1"/>
<pad name="J3.1" x="15.24" y="0" drill="1"/>
<pad name="J1.1" x="-15.24" y="6.35" drill="1"/>
<pad name="J1.2" x="-15.24" y="3.81" drill="1"/>
<pad name="J1.3" x="-15.24" y="1.27" drill="1"/>
<pad name="J1.4" x="-15.24" y="-1.27" drill="1"/>
<pad name="J1.5" x="-15.24" y="-3.81" drill="1"/>
<pad name="J1.6" x="-15.24" y="-6.35" drill="1"/>
<pad name="J2.2" x="-3.81" y="5.08" drill="1"/>
<pad name="J2.1" x="-1.27" y="5.08" drill="1"/>
<wire x1="16.51" y1="8.255" x2="15.875" y2="8.89" width="0.127" layer="21"/>
<wire x1="14.605" y1="8.89" x2="13.97" y2="8.255" width="0.127" layer="21"/>
<wire x1="12.065" y1="8.89" x2="11.43" y2="8.255" width="0.127" layer="21"/>
<wire x1="11.43" y1="8.255" x2="10.795" y2="8.89" width="0.127" layer="21"/>
<wire x1="8.89" y1="8.255" x2="8.255" y2="8.89" width="0.127" layer="21"/>
<wire x1="6.985" y1="8.89" x2="6.35" y2="8.255" width="0.127" layer="21"/>
<wire x1="6.35" y1="8.255" x2="5.715" y2="8.89" width="0.127" layer="21"/>
<wire x1="4.445" y1="8.89" x2="3.81" y2="8.255" width="0.127" layer="21"/>
<wire x1="3.81" y1="8.255" x2="3.175" y2="8.89" width="0.127" layer="21"/>
<wire x1="1.905" y1="8.89" x2="1.27" y2="8.255" width="0.127" layer="21"/>
<wire x1="1.27" y1="8.255" x2="0.635" y2="8.89" width="0.127" layer="21"/>
<wire x1="-0.635" y1="8.89" x2="-1.27" y2="8.255" width="0.127" layer="21"/>
<wire x1="-1.27" y1="8.255" x2="-1.905" y2="8.89" width="0.127" layer="21"/>
<wire x1="-3.175" y1="8.89" x2="-3.81" y2="8.255" width="0.127" layer="21"/>
<wire x1="-3.81" y1="8.255" x2="-4.445" y2="8.89" width="0.127" layer="21"/>
<wire x1="-5.715" y1="8.89" x2="-6.35" y2="8.255" width="0.127" layer="21"/>
<wire x1="-8.255" y1="8.89" x2="-8.89" y2="8.255" width="0.127" layer="21"/>
<wire x1="-8.89" y1="8.255" x2="-9.525" y2="8.89" width="0.127" layer="21"/>
<wire x1="-10.795" y1="8.89" x2="-11.43" y2="8.255" width="0.127" layer="21"/>
<wire x1="-11.43" y1="8.255" x2="-12.065" y2="8.89" width="0.127" layer="21"/>
<wire x1="-13.335" y1="8.89" x2="-13.97" y2="8.255" width="0.127" layer="21"/>
<wire x1="-13.97" y1="8.255" x2="-13.97" y2="6.985" width="0.127" layer="21"/>
<wire x1="-13.97" y1="6.985" x2="-13.335" y2="6.35" width="0.127" layer="21"/>
<wire x1="-13.335" y1="6.35" x2="-12.065" y2="6.35" width="0.127" layer="21"/>
<wire x1="-12.065" y1="6.35" x2="-11.43" y2="6.985" width="0.127" layer="21"/>
<wire x1="-11.43" y1="6.985" x2="-10.795" y2="6.35" width="0.127" layer="21"/>
<wire x1="-10.795" y1="6.35" x2="-9.525" y2="6.35" width="0.127" layer="21"/>
<wire x1="-9.525" y1="6.35" x2="-8.89" y2="6.985" width="0.127" layer="21"/>
<wire x1="-8.89" y1="6.985" x2="-8.255" y2="6.35" width="0.127" layer="21"/>
<wire x1="-8.255" y1="6.35" x2="-6.985" y2="6.35" width="0.127" layer="21"/>
<wire x1="-6.985" y1="6.35" x2="-6.35" y2="6.985" width="0.127" layer="21"/>
<wire x1="-6.35" y1="6.985" x2="-5.715" y2="6.35" width="0.127" layer="21"/>
<wire x1="-5.715" y1="6.35" x2="-4.445" y2="6.35" width="0.127" layer="21"/>
<wire x1="-4.445" y1="6.35" x2="-3.81" y2="6.985" width="0.127" layer="21"/>
<wire x1="-3.81" y1="6.985" x2="-3.175" y2="6.35" width="0.127" layer="21"/>
<wire x1="-3.175" y1="6.35" x2="-1.905" y2="6.35" width="0.127" layer="21"/>
<wire x1="-1.905" y1="6.35" x2="-1.27" y2="6.985" width="0.127" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-0.635" y2="6.35" width="0.127" layer="21"/>
<wire x1="-0.635" y1="6.35" x2="0.635" y2="6.35" width="0.127" layer="21"/>
<wire x1="0.635" y1="6.35" x2="1.27" y2="6.985" width="0.127" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.905" y2="6.35" width="0.127" layer="21"/>
<wire x1="1.905" y1="6.35" x2="3.175" y2="6.35" width="0.127" layer="21"/>
<wire x1="3.175" y1="6.35" x2="3.81" y2="6.985" width="0.127" layer="21"/>
<wire x1="3.81" y1="6.985" x2="4.445" y2="6.35" width="0.127" layer="21"/>
<wire x1="4.445" y1="6.35" x2="5.715" y2="6.35" width="0.127" layer="21"/>
<wire x1="5.715" y1="6.35" x2="6.35" y2="6.985" width="0.127" layer="21"/>
<wire x1="6.35" y1="6.985" x2="6.985" y2="6.35" width="0.127" layer="21"/>
<wire x1="6.985" y1="6.35" x2="8.255" y2="6.35" width="0.127" layer="21"/>
<wire x1="8.255" y1="6.35" x2="8.89" y2="6.985" width="0.127" layer="21"/>
<wire x1="8.89" y1="6.985" x2="9.525" y2="6.35" width="0.127" layer="21"/>
<wire x1="9.525" y1="6.35" x2="10.795" y2="6.35" width="0.127" layer="21"/>
<wire x1="10.795" y1="6.35" x2="11.43" y2="6.985" width="0.127" layer="21"/>
<wire x1="11.43" y1="6.985" x2="12.065" y2="6.35" width="0.127" layer="21"/>
<wire x1="12.065" y1="6.35" x2="13.335" y2="6.35" width="0.127" layer="21"/>
<wire x1="13.335" y1="6.35" x2="13.97" y2="6.985" width="0.127" layer="21"/>
<wire x1="13.97" y1="6.985" x2="14.605" y2="6.35" width="0.127" layer="21"/>
<wire x1="14.605" y1="6.35" x2="15.875" y2="6.35" width="0.127" layer="21"/>
<wire x1="15.875" y1="6.35" x2="16.51" y2="6.985" width="0.127" layer="21"/>
<pad name="J7.1" x="15.24" y="-7.62" drill="1"/>
<pad name="J7.2" x="12.7" y="-7.62" drill="1"/>
<pad name="J7.3" x="10.16" y="-7.62" drill="1"/>
<pad name="J7.4" x="7.62" y="-7.62" drill="1"/>
<pad name="J7.5" x="5.08" y="-7.62" drill="1"/>
<pad name="J7.6" x="2.54" y="-7.62" drill="1"/>
<pad name="J7.8" x="-2.54" y="-7.62" drill="1"/>
<pad name="J7.9" x="-5.08" y="-7.62" drill="1"/>
<pad name="J7.11" x="-10.16" y="-7.62" drill="1"/>
<pad name="J7.12" x="-12.7" y="-7.62" drill="1"/>
<wire x1="16.51" y1="-6.985" x2="15.875" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-13.335" y1="-6.35" x2="-13.97" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-6.985" x2="-13.97" y2="-8.255" width="0.127" layer="21"/>
<wire x1="15.875" y1="-8.89" x2="16.51" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-14.605" y1="7.62" x2="-15.875" y2="7.62" width="0.127" layer="21"/>
<wire x1="-15.875" y1="7.62" x2="-16.51" y2="6.985" width="0.127" layer="21"/>
<wire x1="-16.51" y1="5.715" x2="-15.875" y2="5.08" width="0.127" layer="21"/>
<wire x1="-15.875" y1="5.08" x2="-16.51" y2="4.445" width="0.127" layer="21"/>
<wire x1="-16.51" y1="3.175" x2="-15.875" y2="2.54" width="0.127" layer="21"/>
<wire x1="-15.875" y1="2.54" x2="-16.51" y2="1.905" width="0.127" layer="21"/>
<wire x1="-16.51" y1="0.635" x2="-15.875" y2="0" width="0.127" layer="21"/>
<wire x1="-15.875" y1="0" x2="-16.51" y2="-0.635" width="0.127" layer="21"/>
<wire x1="-16.51" y1="-1.905" x2="-15.875" y2="-2.54" width="0.127" layer="21"/>
<wire x1="-15.875" y1="-2.54" x2="-16.51" y2="-3.175" width="0.127" layer="21"/>
<wire x1="-16.51" y1="-4.445" x2="-15.875" y2="-5.08" width="0.127" layer="21"/>
<wire x1="-15.875" y1="-5.08" x2="-16.51" y2="-5.715" width="0.127" layer="21"/>
<wire x1="-16.51" y1="-6.985" x2="-15.875" y2="-7.62" width="0.127" layer="21"/>
<wire x1="-15.875" y1="-7.62" x2="-14.605" y2="-7.62" width="0.127" layer="21"/>
<wire x1="-14.605" y1="-7.62" x2="-13.97" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-6.985" x2="-13.97" y2="-5.715" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-5.715" x2="-14.605" y2="-5.08" width="0.127" layer="21"/>
<wire x1="-14.605" y1="-5.08" x2="-13.97" y2="-4.445" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-4.445" x2="-13.97" y2="-3.175" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-3.175" x2="-14.605" y2="-2.54" width="0.127" layer="21"/>
<wire x1="-14.605" y1="-2.54" x2="-13.97" y2="-1.905" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-1.905" x2="-13.97" y2="-0.635" width="0.127" layer="21"/>
<wire x1="-13.97" y1="-0.635" x2="-14.605" y2="0" width="0.127" layer="21"/>
<wire x1="-14.605" y1="0" x2="-13.97" y2="0.635" width="0.127" layer="21"/>
<wire x1="-13.97" y1="0.635" x2="-13.97" y2="1.905" width="0.127" layer="21"/>
<wire x1="-13.97" y1="1.905" x2="-14.605" y2="2.54" width="0.127" layer="21"/>
<wire x1="-14.605" y1="2.54" x2="-13.97" y2="3.175" width="0.127" layer="21"/>
<wire x1="-13.97" y1="3.175" x2="-13.97" y2="4.445" width="0.127" layer="21"/>
<wire x1="-13.97" y1="4.445" x2="-14.605" y2="5.08" width="0.127" layer="21"/>
<wire x1="-14.605" y1="5.08" x2="-13.97" y2="5.715" width="0.127" layer="21"/>
<wire x1="-13.97" y1="5.715" x2="-13.97" y2="6.985" width="0.127" layer="21"/>
<wire x1="-13.97" y1="6.985" x2="-14.605" y2="7.62" width="0.127" layer="21"/>
<wire x1="-5.08" y1="5.715" x2="-4.445" y2="6.35" width="0.127" layer="21"/>
<wire x1="-4.445" y1="6.35" x2="-3.175" y2="6.35" width="0.127" layer="21"/>
<wire x1="-3.175" y1="6.35" x2="-2.54" y2="5.715" width="0.127" layer="21"/>
<wire x1="-2.54" y1="5.715" x2="-1.905" y2="6.35" width="0.127" layer="21"/>
<wire x1="-1.905" y1="6.35" x2="-0.635" y2="6.35" width="0.127" layer="21"/>
<wire x1="-0.635" y1="6.35" x2="0" y2="5.715" width="0.127" layer="21"/>
<wire x1="0" y1="5.715" x2="0" y2="4.445" width="0.127" layer="21"/>
<wire x1="0" y1="4.445" x2="-0.635" y2="3.81" width="0.127" layer="21"/>
<wire x1="-0.635" y1="3.81" x2="-1.905" y2="3.81" width="0.127" layer="21"/>
<wire x1="-1.905" y1="3.81" x2="-2.54" y2="4.445" width="0.127" layer="21"/>
<wire x1="-2.54" y1="4.445" x2="-3.175" y2="3.81" width="0.127" layer="21"/>
<wire x1="-3.175" y1="3.81" x2="-4.445" y2="3.81" width="0.127" layer="21"/>
<wire x1="-4.445" y1="3.81" x2="-5.08" y2="4.445" width="0.127" layer="21"/>
<wire x1="-5.08" y1="4.445" x2="-5.08" y2="5.715" width="0.127" layer="21"/>
<wire x1="15.875" y1="6.35" x2="16.51" y2="5.715" width="0.127" layer="21"/>
<wire x1="16.51" y1="4.445" x2="15.875" y2="3.81" width="0.127" layer="21"/>
<wire x1="15.875" y1="3.81" x2="16.51" y2="3.175" width="0.127" layer="21"/>
<wire x1="16.51" y1="1.905" x2="15.875" y2="1.27" width="0.127" layer="21"/>
<wire x1="15.875" y1="1.27" x2="16.51" y2="0.635" width="0.127" layer="21"/>
<wire x1="16.51" y1="-0.635" x2="15.875" y2="-1.27" width="0.127" layer="21"/>
<wire x1="15.875" y1="-1.27" x2="14.605" y2="-1.27" width="0.127" layer="21"/>
<wire x1="14.605" y1="-1.27" x2="13.97" y2="-0.635" width="0.127" layer="21"/>
<wire x1="13.97" y1="-0.635" x2="13.97" y2="0.635" width="0.127" layer="21"/>
<wire x1="13.97" y1="0.635" x2="14.605" y2="1.27" width="0.127" layer="21"/>
<wire x1="14.605" y1="1.27" x2="13.97" y2="1.905" width="0.127" layer="21"/>
<wire x1="13.97" y1="1.905" x2="13.97" y2="3.175" width="0.127" layer="21"/>
<wire x1="13.97" y1="3.175" x2="14.605" y2="3.81" width="0.127" layer="21"/>
<wire x1="14.605" y1="3.81" x2="13.97" y2="4.445" width="0.127" layer="21"/>
<wire x1="13.97" y1="4.445" x2="13.97" y2="5.715" width="0.127" layer="21"/>
<wire x1="13.97" y1="5.715" x2="14.605" y2="6.35" width="0.127" layer="21"/>
<wire x1="-13.335" y1="-8.89" x2="-13.97" y2="-8.255" width="0.127" layer="21"/>
<pad name="J7.10" x="-7.62" y="-7.62" drill="1"/>
<pad name="J7.7" x="0" y="-7.62" drill="1"/>
<wire x1="16.51" y1="8.89" x2="13.335" y2="8.89" width="0.127" layer="21"/>
<wire x1="13.335" y1="8.89" x2="9.525" y2="8.89" width="0.127" layer="21"/>
<wire x1="9.525" y1="8.89" x2="-6.985" y2="8.89" width="0.127" layer="21"/>
<wire x1="-6.985" y1="8.89" x2="-16.51" y2="8.89" width="0.127" layer="21"/>
<wire x1="-16.51" y1="-8.89" x2="-12.065" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-12.065" y1="-8.89" x2="-10.795" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-10.795" y1="-8.89" x2="-9.525" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-9.525" y1="-8.89" x2="-8.255" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-8.255" y1="-8.89" x2="-6.985" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-6.985" y1="-8.89" x2="-5.715" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-5.715" y1="-8.89" x2="-4.445" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-4.445" y1="-8.89" x2="-3.175" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-3.175" y1="-8.89" x2="-1.905" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-1.905" y1="-8.89" x2="-0.635" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-0.635" y1="-8.89" x2="0.635" y2="-8.89" width="0.127" layer="21"/>
<wire x1="0.635" y1="-8.89" x2="1.905" y2="-8.89" width="0.127" layer="21"/>
<wire x1="1.905" y1="-8.89" x2="3.175" y2="-8.89" width="0.127" layer="21"/>
<wire x1="3.175" y1="-8.89" x2="4.445" y2="-8.89" width="0.127" layer="21"/>
<wire x1="4.445" y1="-8.89" x2="5.715" y2="-8.89" width="0.127" layer="21"/>
<wire x1="5.715" y1="-8.89" x2="6.985" y2="-8.89" width="0.127" layer="21"/>
<wire x1="6.985" y1="-8.89" x2="8.255" y2="-8.89" width="0.127" layer="21"/>
<wire x1="8.255" y1="-8.89" x2="9.525" y2="-8.89" width="0.127" layer="21"/>
<wire x1="9.525" y1="-8.89" x2="10.795" y2="-8.89" width="0.127" layer="21"/>
<wire x1="10.795" y1="-8.89" x2="12.065" y2="-8.89" width="0.127" layer="21"/>
<wire x1="12.065" y1="-8.89" x2="13.335" y2="-8.89" width="0.127" layer="21"/>
<wire x1="13.335" y1="-8.89" x2="14.605" y2="-8.89" width="0.127" layer="21"/>
<wire x1="14.605" y1="-8.89" x2="16.51" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-16.51" y1="8.89" x2="-16.51" y2="-8.89" width="0.127" layer="21"/>
<wire x1="16.51" y1="-8.89" x2="16.51" y2="8.89" width="0.127" layer="21"/>
<wire x1="-6.35" y1="8.255" x2="-6.985" y2="8.89" width="0.127" layer="21"/>
<wire x1="13.97" y1="8.255" x2="13.335" y2="8.89" width="0.127" layer="21"/>
<wire x1="8.89" y1="8.255" x2="9.525" y2="8.89" width="0.127" layer="21"/>
<wire x1="-12.065" y1="-8.89" x2="-11.43" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-11.43" y1="-8.255" x2="-10.795" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-9.525" y1="-8.89" x2="-8.89" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-8.89" y1="-8.255" x2="-8.255" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-6.985" y1="-8.89" x2="-6.35" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-6.35" y1="-8.255" x2="-5.715" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-4.445" y1="-8.89" x2="-3.81" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-3.81" y1="-8.255" x2="-3.175" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-1.905" y1="-8.89" x2="-1.27" y2="-8.255" width="0.127" layer="21"/>
<wire x1="-1.27" y1="-8.255" x2="-0.635" y2="-8.89" width="0.127" layer="21"/>
<wire x1="0.635" y1="-8.89" x2="1.27" y2="-8.255" width="0.127" layer="21"/>
<wire x1="1.27" y1="-8.255" x2="1.905" y2="-8.89" width="0.127" layer="21"/>
<wire x1="3.175" y1="-8.89" x2="3.81" y2="-8.255" width="0.127" layer="21"/>
<wire x1="3.81" y1="-8.255" x2="4.445" y2="-8.89" width="0.127" layer="21"/>
<wire x1="5.715" y1="-8.89" x2="6.35" y2="-8.255" width="0.127" layer="21"/>
<wire x1="6.35" y1="-8.255" x2="6.985" y2="-8.89" width="0.127" layer="21"/>
<wire x1="8.255" y1="-8.89" x2="8.89" y2="-8.255" width="0.127" layer="21"/>
<wire x1="8.89" y1="-8.255" x2="9.525" y2="-8.89" width="0.127" layer="21"/>
<wire x1="10.795" y1="-8.89" x2="11.43" y2="-8.255" width="0.127" layer="21"/>
<wire x1="11.43" y1="-8.255" x2="12.065" y2="-8.89" width="0.127" layer="21"/>
<wire x1="-13.335" y1="-6.35" x2="-12.065" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-12.065" y1="-6.35" x2="-11.43" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-11.43" y1="-6.985" x2="-10.795" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-10.795" y1="-6.35" x2="-9.525" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-9.525" y1="-6.35" x2="-8.89" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-8.89" y1="-6.985" x2="-8.255" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-8.255" y1="-6.35" x2="-6.985" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-6.985" y1="-6.35" x2="-6.35" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-6.35" y1="-6.985" x2="-5.715" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-5.715" y1="-6.35" x2="-4.445" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-4.445" y1="-6.35" x2="-3.81" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-3.81" y1="-6.985" x2="-3.175" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-3.175" y1="-6.35" x2="-1.905" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-1.905" y1="-6.35" x2="-1.27" y2="-6.985" width="0.127" layer="21"/>
<wire x1="-1.27" y1="-6.985" x2="-0.635" y2="-6.35" width="0.127" layer="21"/>
<wire x1="-0.635" y1="-6.35" x2="0.635" y2="-6.35" width="0.127" layer="21"/>
<wire x1="0.635" y1="-6.35" x2="1.27" y2="-6.985" width="0.127" layer="21"/>
<wire x1="1.27" y1="-6.985" x2="1.905" y2="-6.35" width="0.127" layer="21"/>
<wire x1="1.905" y1="-6.35" x2="3.175" y2="-6.35" width="0.127" layer="21"/>
<wire x1="3.175" y1="-6.35" x2="3.81" y2="-6.985" width="0.127" layer="21"/>
<wire x1="3.81" y1="-6.985" x2="4.445" y2="-6.35" width="0.127" layer="21"/>
<wire x1="4.445" y1="-6.35" x2="5.715" y2="-6.35" width="0.127" layer="21"/>
<wire x1="5.715" y1="-6.35" x2="6.35" y2="-6.985" width="0.127" layer="21"/>
<wire x1="6.35" y1="-6.985" x2="6.985" y2="-6.35" width="0.127" layer="21"/>
<wire x1="6.985" y1="-6.35" x2="8.255" y2="-6.35" width="0.127" layer="21"/>
<wire x1="8.255" y1="-6.35" x2="8.89" y2="-6.985" width="0.127" layer="21"/>
<wire x1="8.89" y1="-6.985" x2="9.525" y2="-6.35" width="0.127" layer="21"/>
<wire x1="9.525" y1="-6.35" x2="10.795" y2="-6.35" width="0.127" layer="21"/>
<wire x1="10.795" y1="-6.35" x2="11.43" y2="-6.985" width="0.127" layer="21"/>
<wire x1="11.43" y1="-6.985" x2="12.065" y2="-6.35" width="0.127" layer="21"/>
<wire x1="12.065" y1="-6.35" x2="13.335" y2="-6.35" width="0.127" layer="21"/>
<wire x1="13.335" y1="-6.35" x2="13.97" y2="-6.985" width="0.127" layer="21"/>
<wire x1="13.97" y1="-6.985" x2="14.605" y2="-6.35" width="0.127" layer="21"/>
<wire x1="14.605" y1="-6.35" x2="15.875" y2="-6.35" width="0.127" layer="21"/>
<wire x1="14.605" y1="-8.89" x2="13.97" y2="-8.255" width="0.127" layer="21"/>
<wire x1="13.97" y1="-8.255" x2="13.335" y2="-8.89" width="0.127" layer="21"/>
<text x="0" y="10.16" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-10.16" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="ARDUINO-PRO-MINI">
<description>&lt;b&gt;Arduino Pro Mini&lt;/b&gt;</description>
<pin name="D1/TX" x="-17.78" y="12.7" length="middle"/>
<pin name="D0/RX" x="-17.78" y="10.16" length="middle"/>
<pin name="RST.1" x="-17.78" y="7.62" length="middle"/>
<pin name="GND.1" x="-17.78" y="5.08" length="middle" direction="pwr"/>
<pin name="D2" x="-17.78" y="2.54" length="middle"/>
<pin name="D3" x="-17.78" y="0" length="middle"/>
<pin name="D4" x="-17.78" y="-2.54" length="middle"/>
<pin name="D5" x="-17.78" y="-5.08" length="middle"/>
<pin name="D6" x="-17.78" y="-7.62" length="middle"/>
<pin name="D7" x="-17.78" y="-10.16" length="middle"/>
<pin name="D8" x="-17.78" y="-12.7" length="middle"/>
<pin name="D9" x="-17.78" y="-15.24" length="middle"/>
<pin name="RAW" x="17.78" y="12.7" length="middle" direction="pwr" rot="R180"/>
<pin name="GND.2" x="17.78" y="10.16" length="middle" direction="pwr" rot="R180"/>
<pin name="RST.2" x="17.78" y="7.62" length="middle" rot="R180"/>
<pin name="VCC" x="17.78" y="5.08" length="middle" direction="pwr" rot="R180"/>
<pin name="A3" x="17.78" y="2.54" length="middle" rot="R180"/>
<pin name="A2" x="17.78" y="0" length="middle" rot="R180"/>
<pin name="A1" x="17.78" y="-2.54" length="middle" rot="R180"/>
<pin name="A0" x="17.78" y="-5.08" length="middle" rot="R180"/>
<pin name="D13" x="17.78" y="-7.62" length="middle" rot="R180"/>
<pin name="D12" x="17.78" y="-10.16" length="middle" rot="R180"/>
<pin name="D11" x="17.78" y="-12.7" length="middle" rot="R180"/>
<pin name="D10" x="17.78" y="-15.24" length="middle" rot="R180"/>
<pin name="P_GND" x="5.08" y="35.56" length="middle" rot="R270"/>
<pin name="P_CTS" x="2.54" y="35.56" length="middle" rot="R270"/>
<pin name="P_VCC" x="0" y="35.56" length="middle" rot="R270"/>
<pin name="P_RXD" x="-2.54" y="35.56" length="middle" rot="R270"/>
<pin name="P_TXD" x="-5.08" y="35.56" length="middle" rot="R270"/>
<pin name="P_DTR" x="-7.62" y="35.56" length="middle" rot="R270"/>
<wire x1="-12.7" y1="30.48" x2="12.7" y2="30.48" width="0.254" layer="94"/>
<wire x1="12.7" y1="30.48" x2="12.7" y2="-30.48" width="0.254" layer="94"/>
<wire x1="12.7" y1="-30.48" x2="-12.7" y2="-30.48" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-30.48" x2="-12.7" y2="30.48" width="0.254" layer="94"/>
<text x="15.24" y="27.94" size="1.778" layer="95">&gt;NAME</text>
<text x="15.24" y="25.4" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A7" x="17.78" y="-20.32" length="middle" rot="R180"/>
<pin name="A6" x="17.78" y="-22.86" length="middle" rot="R180"/>
<pin name="GND.3" x="17.78" y="-25.4" length="middle" rot="R180"/>
<pin name="A4" x="17.78" y="17.78" length="middle" rot="R180"/>
<pin name="A5" x="17.78" y="20.32" length="middle" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ARDUINO-PRO-MINI-*" prefix="M">
<description>&lt;b&gt;Arduino Pro Mini&lt;/b&gt; and compatible devices
&lt;p&gt;More details available here:&lt;br&gt;
&lt;a href="https://www.arduino.cc/en/Main/ArduinoBoardProMini"&gt;https://www.arduino.cc/en/Main/ArduinoBoardProMini&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;&lt;b&gt;&lt;a href="http://www.ebay.com/sch/arduino+pro+mini"&gt;Click here to find device on ebay.com&lt;/a&gt;&lt;/b&gt;&lt;/p&gt;

&lt;p&gt;&lt;img alt="photo" src="http://www.diymodules.org/img/device-photo.php?name=ARDUINO-PRO-MINI-"&gt;&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="ARDUINO-PRO-MINI" x="0" y="0"/>
</gates>
<devices>
<device name="" package="ARDUINO-PRO-MINI">
<connects>
<connect gate="G$1" pin="A0" pad="J6.8"/>
<connect gate="G$1" pin="A1" pad="J6.7"/>
<connect gate="G$1" pin="A2" pad="J6.6"/>
<connect gate="G$1" pin="A3" pad="J6.5"/>
<connect gate="G$1" pin="A4" pad="J2.1"/>
<connect gate="G$1" pin="A5" pad="J2.2"/>
<connect gate="G$1" pin="A6" pad="J3.2"/>
<connect gate="G$1" pin="A7" pad="J3.3"/>
<connect gate="G$1" pin="D0/RX" pad="J7.11"/>
<connect gate="G$1" pin="D1/TX" pad="J7.12"/>
<connect gate="G$1" pin="D10" pad="J6.12"/>
<connect gate="G$1" pin="D11" pad="J6.11"/>
<connect gate="G$1" pin="D12" pad="J6.10"/>
<connect gate="G$1" pin="D13" pad="J6.9"/>
<connect gate="G$1" pin="D2" pad="J7.8"/>
<connect gate="G$1" pin="D3" pad="J7.7"/>
<connect gate="G$1" pin="D4" pad="J7.6"/>
<connect gate="G$1" pin="D5" pad="J7.5"/>
<connect gate="G$1" pin="D6" pad="J7.4"/>
<connect gate="G$1" pin="D7" pad="J7.3"/>
<connect gate="G$1" pin="D8" pad="J7.2"/>
<connect gate="G$1" pin="D9" pad="J7.1"/>
<connect gate="G$1" pin="GND.1" pad="J7.9"/>
<connect gate="G$1" pin="GND.2" pad="J6.2"/>
<connect gate="G$1" pin="GND.3" pad="J3.1"/>
<connect gate="G$1" pin="P_CTS" pad="J1.2"/>
<connect gate="G$1" pin="P_DTR" pad="J1.6"/>
<connect gate="G$1" pin="P_GND" pad="J1.1"/>
<connect gate="G$1" pin="P_RXD" pad="J1.4"/>
<connect gate="G$1" pin="P_TXD" pad="J1.5"/>
<connect gate="G$1" pin="P_VCC" pad="J1.3"/>
<connect gate="G$1" pin="RAW" pad="J6.1"/>
<connect gate="G$1" pin="RST.1" pad="J7.10"/>
<connect gate="G$1" pin="RST.2" pad="J6.3"/>
<connect gate="G$1" pin="VCC" pad="J6.4"/>
</connects>
<technologies>
<technology name="3.3V"/>
<technology name="5.0V"/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SOIC127P600X175-16N">
<packages>
<package name="SOIC127P600X175-16N">
<circle x="-4.315" y="5.055" radius="0.1" width="0.2" layer="21"/>
<circle x="-4.315" y="5.055" radius="0.1" width="0.2" layer="51"/>
<wire x1="-2" y1="5" x2="2" y2="5" width="0.127" layer="51"/>
<wire x1="-2" y1="-5" x2="2" y2="-5" width="0.127" layer="51"/>
<wire x1="-2" y1="5.07" x2="2" y2="5.07" width="0.127" layer="21"/>
<wire x1="-2" y1="-5.07" x2="2" y2="-5.07" width="0.127" layer="21"/>
<wire x1="-2" y1="5" x2="-2" y2="-5" width="0.127" layer="51"/>
<wire x1="2" y1="5" x2="2" y2="-5" width="0.127" layer="51"/>
<wire x1="-3.71" y1="5.25" x2="3.71" y2="5.25" width="0.05" layer="39"/>
<wire x1="-3.71" y1="-5.25" x2="3.71" y2="-5.25" width="0.05" layer="39"/>
<wire x1="-3.71" y1="5.25" x2="-3.71" y2="-5.25" width="0.05" layer="39"/>
<wire x1="3.71" y1="5.25" x2="3.71" y2="-5.25" width="0.05" layer="39"/>
<text x="-3.71" y="-5.247" size="1.27" layer="27" align="top-left">&gt;VALUE</text>
<text x="-3.71" y="5.247" size="1.27" layer="25">&gt;NAME</text>
<smd name="1" x="-2.605" y="4.445" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="2" x="-2.605" y="3.175" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="3" x="-2.605" y="1.905" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="4" x="-2.605" y="0.635" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="5" x="-2.605" y="-0.635" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="6" x="-2.605" y="-1.905" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="7" x="-2.605" y="-3.175" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="8" x="-2.605" y="-4.445" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="9" x="2.605" y="-4.445" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="10" x="2.605" y="-3.175" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="11" x="2.605" y="-1.905" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="12" x="2.605" y="-0.635" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="13" x="2.605" y="0.635" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="14" x="2.605" y="1.905" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="15" x="2.605" y="3.175" dx="1.71" dy="0.61" layer="1" roundness="25"/>
<smd name="16" x="2.605" y="4.445" dx="1.71" dy="0.61" layer="1" roundness="25"/>
</package>
</packages>
<symbols>
</symbols>
<devicesets>
<deviceset name="SOIC127P600X175-16N" prefix="U">
<gates>
</gates>
<devices>
<device name="" package="SOIC127P600X175-16N">
<technologies>
<technology name="">
<attribute name="A1_MIN" value="0.15"/>
<attribute name="A_MAX" value="1.75"/>
<attribute name="A_MIN" value="1.75"/>
<attribute name="A_NOM" value="1.75"/>
<attribute name="B_MAX" value="0.51"/>
<attribute name="B_MIN" value="0.38"/>
<attribute name="B_NOM" value="0.445"/>
<attribute name="D1_MAX" value=""/>
<attribute name="D1_MIN" value=""/>
<attribute name="D1_NOM" value=""/>
<attribute name="D2_MAX" value="0.0"/>
<attribute name="DMAX" value=""/>
<attribute name="DMIN" value=""/>
<attribute name="DNOM" value=""/>
<attribute name="D_MAX" value="10.0"/>
<attribute name="D_MIN" value="9.8"/>
<attribute name="D_NOM" value="9.9"/>
<attribute name="E1_MAX" value="4.0"/>
<attribute name="E1_MIN" value="3.8"/>
<attribute name="E1_NOM" value="3.9"/>
<attribute name="E2_MAX" value="0.0"/>
<attribute name="EMAX" value=""/>
<attribute name="EMIN" value=""/>
<attribute name="ENOM" value="1.27"/>
<attribute name="E_MAX" value="6.2"/>
<attribute name="E_MIN" value="5.8"/>
<attribute name="E_NOM" value="6.0"/>
<attribute name="L1_MAX" value=""/>
<attribute name="L1_MIN" value=""/>
<attribute name="L1_NOM" value=""/>
<attribute name="L_MAX" value="0.93"/>
<attribute name="L_MIN" value="0.5"/>
<attribute name="L_NOM" value="0.715"/>
<attribute name="MANUFACTURER" value="Vishay"/>
<attribute name="PACKAGE_TYPE" value=""/>
<attribute name="PINS" value=""/>
<attribute name="PIN_COUNT" value="16.0"/>
<attribute name="SNAPEDA_PACKAGE_ID" value="6728"/>
<attribute name="STANDARD" value="IPC 7351B"/>
<attribute name="VACANCIES" value=""/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SOP63P600X175-24N">
<packages>
<package name="SOP63P600X175-24N">
<text x="-3.85" y="-4.502" size="1.27" layer="27" align="top-left">&gt;VALUE</text>
<text x="-3.85" y="4.502" size="1.27" layer="25">&gt;NAME</text>
<circle x="-4.355" y="3.9025" radius="0.2" width="0" layer="21"/>
<circle x="-4.355" y="3.9025" radius="0.2" width="0" layer="51"/>
<wire x1="-1.95" y1="4.325" x2="1.95" y2="4.325" width="0.127" layer="51"/>
<wire x1="-1.95" y1="-4.325" x2="1.95" y2="-4.325" width="0.127" layer="51"/>
<wire x1="-1.95" y1="4.325" x2="1.95" y2="4.325" width="0.127" layer="21"/>
<wire x1="-1.95" y1="-4.325" x2="1.95" y2="-4.325" width="0.127" layer="21"/>
<wire x1="-1.95" y1="4.325" x2="-1.95" y2="-4.325" width="0.127" layer="51"/>
<wire x1="1.95" y1="4.325" x2="1.95" y2="-4.325" width="0.127" layer="51"/>
<wire x1="-3.655" y1="4.575" x2="3.655" y2="4.575" width="0.05" layer="39"/>
<wire x1="-3.655" y1="-4.575" x2="3.655" y2="-4.575" width="0.05" layer="39"/>
<wire x1="-3.655" y1="4.575" x2="-3.655" y2="-4.575" width="0.05" layer="39"/>
<wire x1="3.655" y1="4.575" x2="3.655" y2="-4.575" width="0.05" layer="39"/>
<smd name="1" x="-2.455" y="3.4925" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="2" x="-2.455" y="2.8575" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="3" x="-2.455" y="2.2225" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="4" x="-2.455" y="1.5875" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="5" x="-2.455" y="0.9525" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="6" x="-2.455" y="0.3175" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="7" x="-2.455" y="-0.3175" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="8" x="-2.455" y="-0.9525" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="9" x="-2.455" y="-1.5875" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="10" x="-2.455" y="-2.2225" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="11" x="-2.455" y="-2.8575" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="12" x="-2.455" y="-3.4925" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="13" x="2.455" y="-3.4925" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="14" x="2.455" y="-2.8575" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="15" x="2.455" y="-2.2225" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="16" x="2.455" y="-1.5875" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="17" x="2.455" y="-0.9525" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="18" x="2.455" y="-0.3175" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="19" x="2.455" y="0.3175" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="20" x="2.455" y="0.9525" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="21" x="2.455" y="1.5875" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="22" x="2.455" y="2.2225" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="23" x="2.455" y="2.8575" dx="1.9" dy="0.41" layer="1" roundness="25"/>
<smd name="24" x="2.455" y="3.4925" dx="1.9" dy="0.41" layer="1" roundness="25"/>
</package>
</packages>
<symbols>
</symbols>
<devicesets>
<deviceset name="SOP63P600X175-24N" prefix="P">
<gates>
</gates>
<devices>
<device name="" package="SOP63P600X175-24N">
<technologies>
<technology name="">
<attribute name="A1_MAX" value="None"/>
<attribute name="A1_MIN" value="0.2"/>
<attribute name="A1_NOM" value="None"/>
<attribute name="A_MAX" value="1.75"/>
<attribute name="A_MIN" value="None"/>
<attribute name="A_NOM" value="None"/>
<attribute name="BALL_COLUMNS" value=""/>
<attribute name="BALL_ROWS" value=""/>
<attribute name="BODY_DIAMETER" value=""/>
<attribute name="B_MAX" value="0.3"/>
<attribute name="B_MIN" value="0.2"/>
<attribute name="B_NOM" value="0.25"/>
<attribute name="D1_MAX" value=""/>
<attribute name="D1_MIN" value=""/>
<attribute name="D1_NOM" value=""/>
<attribute name="D2_MAX" value="0.0"/>
<attribute name="D2_MIN" value="None"/>
<attribute name="D2_NOM" value="None"/>
<attribute name="DMAX" value=""/>
<attribute name="DMIN" value=""/>
<attribute name="DNOM" value=""/>
<attribute name="D_MAX" value="8.65"/>
<attribute name="D_MIN" value="8.65"/>
<attribute name="D_NOM" value="8.65"/>
<attribute name="E1_MAX" value="3.9"/>
<attribute name="E1_MIN" value="3.9"/>
<attribute name="E1_NOM" value="3.9"/>
<attribute name="E2_MAX" value="0.0"/>
<attribute name="E2_MIN" value="None"/>
<attribute name="E2_NOM" value="None"/>
<attribute name="EMAX" value=""/>
<attribute name="EMIN" value=""/>
<attribute name="ENOM" value="0.635"/>
<attribute name="E_MAX" value="6.0"/>
<attribute name="E_MIN" value="6.0"/>
<attribute name="E_NOM" value="6.0"/>
<attribute name="IPC" value=""/>
<attribute name="JEDEC" value=""/>
<attribute name="L1_MAX" value=""/>
<attribute name="L1_MIN" value=""/>
<attribute name="L1_NOM" value=""/>
<attribute name="L_MAX" value="1.27"/>
<attribute name="L_MIN" value="0.4"/>
<attribute name="L_NOM" value="0.835"/>
<attribute name="MANUFACTURER" value="Silicon Labs"/>
<attribute name="PACKAGE_TYPE" value=""/>
<attribute name="PINS" value=""/>
<attribute name="PIN_COLUMNS" value=""/>
<attribute name="PIN_COUNT" value="24.0"/>
<attribute name="SNAPEDA_PACKAGE_ID" value="29164"/>
<attribute name="THERMAL_PAD" value=""/>
<attribute name="VACANCIES" value=""/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<modules>
<module name="TESTE" prefix="" dx="30.48" dy="20.32">
<ports>
</ports>
<variantdefs>
</variantdefs>
<parts>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</module>
</modules>
<parts>
<part name="M1" library="diy-modules" deviceset="ARDUINO-PRO-MINI-*" device="" technology="3.3V"/>
<part name="P1" library="SOP63P600X175-24N" deviceset="SOP63P600X175-24N" device=""/>
<part name="P2" library="SOP63P600X175-24N" deviceset="SOP63P600X175-24N" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="144.78" y="73.66" size="1.778" layer="91">to SI4844 pin 16</text>
<text x="144.78" y="66.04" size="1.778" layer="91">to SI4844 pin 17</text>
</plain>
<instances>
<instance part="M1" gate="G$1" x="78.74" y="48.26" smashed="yes">
<attribute name="NAME" x="93.98" y="76.2" size="1.778" layer="95"/>
<attribute name="VALUE" x="93.98" y="73.66" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="SDA" class="0">
<segment>
<pinref part="M1" gate="G$1" pin="A4"/>
<wire x1="96.52" y1="66.04" x2="139.7" y2="66.04" width="0.1524" layer="91"/>
<label x="139.7" y="68.58" size="1.778" layer="95"/>
<label x="139.7" y="68.58" size="1.778" layer="95"/>
</segment>
</net>
<net name="SCL" class="0">
<segment>
<pinref part="M1" gate="G$1" pin="A5"/>
<wire x1="96.52" y1="68.58" x2="96.52" y2="71.12" width="0.1524" layer="91"/>
<wire x1="96.52" y1="71.12" x2="139.7" y2="71.12" width="0.1524" layer="91"/>
<label x="139.7" y="73.66" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
