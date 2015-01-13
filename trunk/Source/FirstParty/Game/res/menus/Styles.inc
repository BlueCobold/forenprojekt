<styles>
	<buttonStyle name="default">
		<mouseRect left="2" top="2" width="306" height="46" />
		<idle spriteSheet="gui_elements" sprite="Button_idle"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_elements" sprite="Button_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="Button_pressed"
			offsetx="-6" offsety="-6" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5" />
	</buttonStyle>
	<buttonStyle name="square">
		<mouseRect left="2" top="2" width="36" height="36" />
		<idle spriteSheet="gui_elements" sprite="normal"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="-2" fontoffsety="1" />
		<hover spriteSheet="gui_elements" sprite="hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="-2" fontoffsety="1" />
		<pressed spriteSheet="gui_elements" sprite="pressed"
			offsetx="-4" offsety="-4" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="2" />
	</buttonStyle>
	<buttonStyle name="coins">
		<mouseRect left="2" top="2" width="40" height="42" />
		<idle spriteSheet="gui_elements" sprite="coins_idle"
			offsetx="3" offsety="3"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_elements" sprite="coins_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="coins_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
	</buttonStyle>
	<buttonStyle name="goblet">
		<mouseRect left="3" top="3" width="36" height="30" />
		<idle spriteSheet="gui_elements" sprite="goblet_idle"
			offsetx="2" offsety="3"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_elements" sprite="goblet_hover"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="goblet_hover"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
	</buttonStyle>
	<buttonStyle name="small">
		<mouseRect left="2" top="2" width="141" height="46" />
		<idle spriteSheet="gui_elements" sprite="Button_small_idle"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_elements" sprite="Button_small_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="Button_small_pressed"
			offsetx="-6" offsety="-6" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5" />
	</buttonStyle>
	<buttonStyle name="selectLevel">
		<mouseRect left="0" top="0" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<buttonStyle name="arrowleft">
		<mouseRect left="2" top="2" width="54" height="38" />
		<idle spriteSheet="gui_elements" sprite="left_idle"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="left_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="left_pressed"
			offsetx="-5" offsety="-5" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<buttonStyle name="arrowright">
		<mouseRect left="2" top="2" width="54" height="38" />
		<idle spriteSheet="gui_elements" sprite="right_idle"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="right_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_elements" sprite="right_pressed"
			offsetx="-5" offsety="-5" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<checkboxStyle name="default">
		<mouseRect left="1" top="5" width="27" height="27" />
		<unchecked spriteSheet="gui_elements" sprite="unchecked_idle"
			offsetx="0" offsety="4" />
		<checked spriteSheet="gui_elements" sprite="checked_idle"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_elements" sprite="checked_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<checkboxStyle name="timemode">
		<mouseRect left="1" top="1" width="34" height="34" />
		<unchecked spriteSheet="gui_elements" sprite="target_unchecked"
			offsetx="0" offsety="0" />
		<checked spriteSheet="gui_elements" sprite="target_checked"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_elements" sprite="target_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<checkboxStyle name="targetmode">
		<mouseRect left="1" top="1" width="34" height="34" />
		<unchecked spriteSheet="gui_elements" sprite="time_unchecked"
			offsetx="0" offsety="0" />
		<checked spriteSheet="gui_elements" sprite="time_checked"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_elements" sprite="time_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<checkboxStyle name="offlineHighscore">
		<mouseRect left="1" top="1" width="34" height="34" />
		<unchecked spriteSheet="gui_elements" sprite="offline_unchecked"
			offsetx="0" offsety="0" />
		<checked spriteSheet="gui_elements" sprite="offline_checked"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_elements" sprite="offline_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<checkboxStyle name="onlineHighscore">
		<mouseRect left="1" top="1" width="34" height="34" />
		<unchecked spriteSheet="gui_elements" sprite="online_unchecked"
			offsetx="0" offsety="0" />
		<checked spriteSheet="gui_elements" sprite="online_checked"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_elements" sprite="online_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<sliderStyle name="default">
		<mouseRect left="8" top="-3" width="26" height="36" />
		<value min="0" max="100" width="83"/>
		<idle backgroundspriteSheet="gui_elements" backgroundsprite="bar_idle"
			backgroundoffsetx="8" backgroundoffsety="8" 
			sliderspriteSheet="gui_elements" slidersprite="button_idle"
			slideroffsetx="0" slideroffsety="-1" />
		<active
 backgroundspriteSheet="gui_elements" backgroundsprite="bar_active"
			backgroundoffsetx="0" backgroundoffsety="0" 
			sliderspriteSheet="gui_elements" slidersprite="button_active"
			slideroffsetx="-6" slideroffsety="-9" />
	</sliderStyle>
	<tooltipStyle name="default" offsetx="0" offsety="45">
		<text font="gold" offsetx="0" offsety="0"/>
		<background id="1" spriteSheet="gui_elements" sprite="toolTip" width="10" height="10" />
		<background id="2" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" width="10" height="10" />
		<background id="3" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" width="10" height="10" />
		<background id="4" spriteSheet="gui_elements" sprite="toolTip" srcyoffset="10" width="10" height="10" />
		<background id="5" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" srcyoffset="10" width="10" height="10" />
		<background id="6" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" srcyoffset="10" width="10" height="10" />
		<background id="7" spriteSheet="gui_elements" sprite="toolTip" srcyoffset="20" width="10" height="10" />
		<background id="8" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" srcyoffset="20" width="10" height="10" />
		<background id="9" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" srcyoffset="20" width="10" height="10" />
	</tooltipStyle>
	<inputboxStyle name="default">
		<text font="buttonHoverFont" offsetx="5" offsety="0" />
		<background id="1" spriteSheet="gui_elements" sprite="toolTip" width="10" height="10" />
		<background id="2" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" width="10" height="10" />
		<background id="3" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" width="10" height="10" />
		<background id="4" spriteSheet="gui_elements" sprite="toolTip" srcyoffset="10" width="10" height="10" />
		<background id="5" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" srcyoffset="10" width="10" height="10" />
		<background id="6" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" srcyoffset="10" width="10" height="10" />
		<background id="7" spriteSheet="gui_elements" sprite="toolTip" srcyoffset="20" width="10" height="10" />
		<background id="8" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="10" srcyoffset="20" width="10" height="10" />
		<background id="9" spriteSheet="gui_elements" sprite="toolTip" srcxoffset="20" srcyoffset="20" width="10" height="10" />
	</inputboxStyle>
</styles>