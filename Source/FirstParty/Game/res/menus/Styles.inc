<styles>
	<buttonStyle name="default">
		<mouseRect left="2" top="2" width="306" height="46" />
		<idle texture="GuiElements" srcx="0" srcy="0" width="317" height="58"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover texture="GuiElements" srcx="317" srcy="0" width="317" height="58"
			offsetx="0" offsety="0"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed texture="GuiElements" srcx="634" srcy="0" width="325" height="66"
			offsetx="-6" offsety="-6"
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5" />
	</buttonStyle>
	<checkboxStyle name="default">
		<mouseRect left="1" top="5" width="27" height="27" />
		<unchecked texture="GuiElements" srcx="704" srcy="66" width="38" height="37"
			offsetx="0" offsety="4" />
		<checked texture="GuiElements" srcx="962" srcy="41" width="38" height="41"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<sliderStyle name="default">
		<mouseRect left="8" top="-3" width="26" height="36" />
		<value min="0" max="100" width="83"/>
		<idle backroundtexture="GuiElements" backroundsrcx="742" backroundsrcy="96" 
			backroundwidth="118" backroundheight="22" 
			backroundoffsetx="8" backroundoffsety="8" 
			slidertexture="GuiElements" slidersrcx="860" slidersrcy="104" 
			sliderwidth="32" sliderheight="46" 
			slideroffsetx="0" slideroffsety="-1" />
		<active backroundtexture="GuiElements" backroundsrcx="742" backroundsrcy="66" 
			backroundwidth="124" backroundheight="30" 
			backroundoffsetx="0" backroundoffsety="0" 
			slidertexture="GuiElements" slidersrcx="984" slidersrcy="82" 
			sliderwidth="38" sliderheight="54" 
			slideroffsetx="-6" slideroffsety="-9" />
	</sliderStyle>
	<tooltipStyle name="default" offsetx="0" offsety="20">
		<text font="gold" offsetx="0" offsety="5"/>
		<left texture="tooltip" srcx="0" srcy="0" width="5" height="32" />
		<center texture="tooltip" srcx="6" srcy="0" width="32" height="32" />
		<right texture="tooltip" srcx="123" srcy="0" width="5" height="32" />
	</tooltipStyle>
</styles>