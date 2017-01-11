<styles>
	<buttonStyle name="achieveButtonStyle">
		<mouseRect left="10" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Star"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Star">
					<layout>
						<sprites>Icon_Star</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Star_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover" copies="11" blending="premul">
					<layout>
						<sprites>
							Icon_Star_hover_small
						</sprites>
					</layout>
					<position axis="y">
						<mul>
							<static value="-50"/>
							<sine>
								<mul>
									<static value="90" />
									<mod>
										<add>
											<time />
											<cache>
												<mul>
													<variable name="cloneId"/>
													<static value="3.36"/>
												</mul>
											</cache>
										</add>
										<static value="1"/>
									</mod>
								</mul>
							</sine>
						</mul>
					</position>
					<position axis="x">
						<mul>
							<static value="-10"/>
							<sub>
								<variable name="cloneId" />
								<static value="5" />
							</sub>
							<mod>
								<add>
									<time />
									<cache>
										<mul>
											<variable name="cloneId"/>
											<static value="3.36"/>
										</mul>
									</cache>
								</add>
								<static value="1"/>
							</mod>
						</mul>
					</position>
					<color channel="alpha">
						<sub>
							<static value="1"/>
							<ramp min="0.7" max="1">
								<mod>
									<add>
										<time />
										<cache>
											<mul>
												<variable name="cloneId"/>
												<static value="3.36"/>
											</mul>
										</cache>
									</add>
									<static value="1"/>
								</mod>
							</ramp>
						</sub>
					</color>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover" blending="premul">
					<layout>
						<sprites>Icon_Star_hover</sprites>
					</layout>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Star_pressed"
			offsetx="2" offsety="2" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover" copies="11" blending="premul">
					<layout>
						<sprites>
							Icon_Star_pressed_small
						</sprites>
					</layout>
					<position axis="y">
						<mul>
							<static value="-50"/>
							<sine>
								<mul>
									<static value="90" />
									<mod>
										<add>
											<time />
											<cache>
												<mul>
													<variable name="cloneId"/>
													<static value="3.36"/>
												</mul>
											</cache>
										</add>
										<static value="1"/>
									</mod>
								</mul>
							</sine>
						</mul>
					</position>
					<position axis="x">
						<mul>
							<static value="-10"/>
							<sub>
								<variable name="cloneId" />
								<static value="5" />
							</sub>
							<mod>
								<add>
									<time />
									<cache>
										<mul>
											<variable name="cloneId"/>
											<static value="3.36"/>
										</mul>
									</cache>
								</add>
								<static value="1"/>
							</mod>
						</mul>
					</position>
					<color channel="alpha">
						<sub>
							<static value="1"/>
							<ramp min="0.7" max="1">
								<mod>
									<add>
										<time />
										<cache>
											<mul>
												<variable name="cloneId"/>
												<static value="3.36"/>
											</mul>
										</cache>
									</add>
									<static value="1"/>
								</mod>
							</ramp>
						</sub>
					</color>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover" blending="premul">
					<layout>
						<sprites>Icon_Star_pressed</sprites>
					</layout>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="playButtonStyle">
		<mouseRect left="-30" top="-55" width="100" height="90" />
		<idle spriteSheet="gui_animations" sprite="Icon_Play_hover_ball00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_ball00" blending="premul">
					<layout>
						<sprites>Icon_Play00</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Play_hover_ball00"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_ball00" blending="premul">
					<layout>
						<sprites>
							Icon_Play_hover_ball00,Icon_Play_hover_ball01,Icon_Play_hover_ball02,Icon_Play_hover_ball03,
							Icon_Play_hover_ball04,Icon_Play_hover_ball05,Icon_Play_hover_ball06,Icon_Play_hover_ball07,
							Icon_Play_hover_ball08,Icon_Play_hover_ball09,Icon_Play_hover_ball10,Icon_Play_hover_ball11,
							Icon_Play_hover_ball12,Icon_Play_hover_ball13,Icon_Play_hover_ball14,Icon_Play_hover_ball15,
							Icon_Play_hover_ball16,Icon_Play_hover_ball17,Icon_Play_hover_ball18,Icon_Play_hover_ball19,
							Icon_Play_hover_ball20,
							Icon_Play_hover_ball21,Icon_Play_hover_ball20,Icon_Play_hover_ball19,Icon_Play_hover_ball18,
							Icon_Play_hover_ball17,Icon_Play_hover_ball16,Icon_Play_hover_ball15,Icon_Play_hover_ball14,
							Icon_Play_hover_ball13,Icon_Play_hover_ball12,Icon_Play_hover_ball11,Icon_Play_hover_ball10,
							Icon_Play_hover_ball09,Icon_Play_hover_ball08,Icon_Play_hover_ball07,Icon_Play_hover_ball06,
							Icon_Play_hover_ball05,Icon_Play_hover_ball04,Icon_Play_hover_ball03,Icon_Play_hover_ball02,
							Icon_Play_hover_ball01
						</sprites>
					</layout>
					<frameindex frames="42">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_teeter00" blending="premul">
					<layout>
						<sprites>
							Icon_Play_hover_teeter00,Icon_Play_hover_teeter01,Icon_Play_hover_teeter02,Icon_Play_hover_teeter03,
							Icon_Play_hover_teeter04,Icon_Play_hover_teeter05,Icon_Play_hover_teeter06,Icon_Play_hover_teeter07,
							Icon_Play_hover_teeter08,Icon_Play_hover_teeter09,Icon_Play_hover_teeter10,Icon_Play_hover_teeter10,
							Icon_Play_hover_teeter09,Icon_Play_hover_teeter08,Icon_Play_hover_teeter07,Icon_Play_hover_teeter06,
							Icon_Play_hover_teeter05,Icon_Play_hover_teeter04,Icon_Play_hover_teeter03,Icon_Play_hover_teeter02,
							Icon_Play_hover_teeter01,
							Icon_Play_hover_teeter00,Icon_Play_hover_teeter22,Icon_Play_hover_teeter23,Icon_Play_hover_teeter24,
							Icon_Play_hover_teeter25,Icon_Play_hover_teeter26,Icon_Play_hover_teeter27,Icon_Play_hover_teeter28,
							Icon_Play_hover_teeter29,Icon_Play_hover_teeter30,Icon_Play_hover_teeter31,Icon_Play_hover_teeter31,
							Icon_Play_hover_teeter30,Icon_Play_hover_teeter29,Icon_Play_hover_teeter28,Icon_Play_hover_teeter27,
							Icon_Play_hover_teeter26,Icon_Play_hover_teeter25,Icon_Play_hover_teeter24,Icon_Play_hover_teeter23,
							Icon_Play_hover_teeter22
						</sprites>
					</layout>
					<frameindex frames="42">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Play_pressed00"
			offsetx="2" offsety="2" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Play_pressed00" blending="premul">
					<layout>
						<sprites>
							Icon_Play_pressed00,Icon_Play_pressed01,Icon_Play_pressed02,Icon_Play_pressed03,
							Icon_Play_pressed04,Icon_Play_pressed05,Icon_Play_pressed06,Icon_Play_pressed07,
							Icon_Play_pressed08,Icon_Play_pressed09,Icon_Play_pressed10,Icon_Play_pressed11,
							Icon_Play_pressed12,Icon_Play_pressed13,Icon_Play_pressed14,Icon_Play_pressed15,
							Icon_Play_pressed16,Icon_Play_pressed17,Icon_Play_pressed18,Icon_Play_pressed19,
							Icon_Play_pressed20,Icon_Play_pressed21,Icon_Play_pressed22,Icon_Play_pressed23,
							Icon_Play_pressed24,Icon_Play_pressed25,Icon_Play_pressed26,Icon_Play_pressed27,
							Icon_Play_pressed28,Icon_Play_pressed29,Icon_Play_pressed30,Icon_Play_pressed31,
							Icon_Play_pressed32,Icon_Play_pressed33,Icon_Play_pressed34,Icon_Play_pressed35,
							Icon_Play_pressed36,Icon_Play_pressed37,Icon_Play_pressed38,Icon_Play_pressed39,
							Icon_Play_pressed40,Icon_Play_pressed41
						</sprites>
					</layout>
					<frameindex frames="42">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="selectButtonStyle">
		<mouseRect left="0" top="-40" width="100" height="75" />
		<idle spriteSheet="gui_animations" sprite="Icon_Select00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Select00" blending="premul">
					<layout>
						<sprites>Icon_Select00</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Select_Hover_base"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Select_Hover_base" blending="premul"/>
				<animation spritesheet="gui_animations" sprite="Icon_Select_Hover_base" x="-3" y="-5" blending="premul">
					<layout>
						<sprites>
							Icon_Select_Hover00,Icon_Select_Hover01,Icon_Select_Hover02,Icon_Select_Hover03,
							Icon_Select_Hover04,Icon_Select_Hover05,Icon_Select_Hover06,Icon_Select_Hover07,
							Icon_Select_Hover08,Icon_Select_Hover09,Icon_Select_Hover10,Icon_Select_Hover11,
							Icon_Select_Hover12,Icon_Select_Hover13,Icon_Select_Hover14,Icon_Select_Hover15,
							Icon_Select_Hover16,Icon_Select_Hover17,Icon_Select_Hover18,Icon_Select_Hover19,
							Icon_Select_Hover20,Icon_Select_Hover21,Icon_Select_Hover22,Icon_Select_Hover23,
							Icon_Select_Hover24,Icon_Select_Hover25,Icon_Select_Hover26,
							Icon_Select_Hover26,Icon_Select_Hover25,Icon_Select_Hover24,Icon_Select_Hover23,
							Icon_Select_Hover22,Icon_Select_Hover21,Icon_Select_Hover20,Icon_Select_Hover19,
							Icon_Select_Hover18,Icon_Select_Hover17,Icon_Select_Hover16,Icon_Select_Hover15,
							Icon_Select_Hover14,Icon_Select_Hover13,Icon_Select_Hover12,Icon_Select_Hover11,
							Icon_Select_Hover10,Icon_Select_Hover09,Icon_Select_Hover08,Icon_Select_Hover07,
							Icon_Select_Hover06,Icon_Select_Hover05,Icon_Select_Hover04,Icon_Select_Hover03,
							Icon_Select_Hover02,Icon_Select_Hover01,Icon_Select_Hover00
						</sprites>
					</layout>
					<frameindex frames="54">
						<delay start="53" duration="10">
							<delay start="26" duration="10">
								<mod>
									<mul>
										<time />
										<static value="30"/>
									</mul>
									<static value="74" /><!-- 54+10+10 -->
								</mod>
							</delay>
						</delay>
					</frameindex>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Select_pressed_base"
			offsetx="-1" offsety="-3" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Select_pressed_base" blending="premul"/>
				<animation spritesheet="gui_animations" sprite="Icon_Select_pressed_base" x="0" y="0" blending="premul">
					<layout>
						<sprites>
							Icon_Select_pressed00,Icon_Select_pressed01,Icon_Select_pressed02,Icon_Select_pressed03,
							Icon_Select_pressed04,Icon_Select_pressed05,Icon_Select_pressed06,Icon_Select_pressed07,
							Icon_Select_pressed08,Icon_Select_pressed09,Icon_Select_pressed10,Icon_Select_pressed11,
							Icon_Select_pressed12,Icon_Select_pressed13,Icon_Select_pressed14,Icon_Select_pressed15,
							Icon_Select_pressed16,Icon_Select_pressed17,Icon_Select_pressed18,Icon_Select_pressed19,
							Icon_Select_pressed20,Icon_Select_pressed21,Icon_Select_pressed22,Icon_Select_pressed23,
							Icon_Select_pressed24,Icon_Select_pressed25,Icon_Select_pressed26,
							Icon_Select_pressed26,Icon_Select_pressed25,Icon_Select_pressed24,Icon_Select_pressed23,
							Icon_Select_pressed22,Icon_Select_pressed21,Icon_Select_pressed20,Icon_Select_pressed19,
							Icon_Select_pressed18,Icon_Select_pressed17,Icon_Select_pressed16,Icon_Select_pressed15,
							Icon_Select_pressed14,Icon_Select_pressed13,Icon_Select_pressed12,Icon_Select_pressed11,
							Icon_Select_pressed10,Icon_Select_pressed09,Icon_Select_pressed08,Icon_Select_pressed07,
							Icon_Select_pressed06,Icon_Select_pressed05,Icon_Select_pressed04,Icon_Select_pressed03,
							Icon_Select_pressed02,Icon_Select_pressed01,Icon_Select_pressed00
						</sprites>
					</layout>
					<frameindex frames="54">
						<delay start="53" duration="10">
							<delay start="26" duration="10">
								<mod>
									<mul>
										<time />
										<static value="30"/>
									</mul>
									<static value="74" /><!-- 54+10+10 -->
								</mod>
							</delay>
						</delay>
					</frameindex>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="optionButtonStyle">
		<mouseRect left="0" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Wheels00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Wheels00" blending="premul">
					<layout>
						<sprites>Icon_Wheels00</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Wheels_hover00"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Wheels_hover00" blending="premul">
					<layout>
						<sprites>
							Icon_Wheels_hover00,Icon_Wheels_hover01,Icon_Wheels_hover02,Icon_Wheels_hover03,
							Icon_Wheels_hover04,Icon_Wheels_hover05,Icon_Wheels_hover06,Icon_Wheels_hover07,
							Icon_Wheels_hover08,Icon_Wheels_hover09,Icon_Wheels_hover10,Icon_Wheels_hover11
						</sprites>
					</layout>
					<frameindex frames="12">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Wheels_pressed00"
			offsetx="2" offsety="2" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Wheels_pressed00" blending="premul">
					<layout>
						<sprites>
							Icon_Wheels_pressed00,Icon_Wheels_pressed01,Icon_Wheels_pressed02,Icon_Wheels_pressed03,
							Icon_Wheels_pressed04,Icon_Wheels_pressed05,Icon_Wheels_pressed06,Icon_Wheels_pressed07,
							Icon_Wheels_pressed08,Icon_Wheels_pressed09,Icon_Wheels_pressed10,Icon_Wheels_pressed11
						</sprites>
					</layout>
					<frameindex frames="12">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="exitButtonStyle">
		<mouseRect left="10" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Door00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Door00" blending="premul">
					<layout>
						<sprites>Icon_Door00</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Door_hover00"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover00" blending="premul">
					<layout>
						<sprites>
							Icon_Door_hover00,Icon_Door_hover01,Icon_Door_hover02,Icon_Door_hover03,
							Icon_Door_hover04,Icon_Door_hover05,Icon_Door_hover06,Icon_Door_hover07,
							Icon_Door_hover08,
							Icon_Door_hover07,Icon_Door_hover06,Icon_Door_hover05,Icon_Door_hover04,
							Icon_Door_hover03,Icon_Door_hover02,Icon_Door_hover01,Icon_Door_hover00
						</sprites>
					</layout>
					<frameindex frames="17">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Door_pressed00"
			offsetx="2" offsety="2" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed00" blending="premul">
					<layout>
						<sprites>
							Icon_Door_pressed00,Icon_Door_pressed01,Icon_Door_pressed02,Icon_Door_pressed03,
							Icon_Door_pressed04,Icon_Door_pressed05,Icon_Door_pressed06,Icon_Door_pressed07,
							Icon_Door_pressed08,
							Icon_Door_pressed07,Icon_Door_pressed06,Icon_Door_pressed05,Icon_Door_pressed04,
							Icon_Door_pressed03,Icon_Door_pressed02,Icon_Door_pressed01,Icon_Door_pressed00
						</sprites>
					</layout>
					<frameindex frames="17">
						<mul>
							<time />
							<static value="30"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="creditButtonStyle">
		<mouseRect left="5" top="-45" width="100" height="95" />
		<idle spriteSheet="gui_animations" sprite="Icon_Monitor00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Monitor00" blending="premul">
					<layout>
						<sprites>Icon_Monitor00</sprites>
					</layout>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Monitor_hover00"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_hover00" blending="premul"/>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_hover00" x="-7" y="5" blending="premul">
					<layout>
						<sprites>
							Icon_Monitor_hover00,Icon_Monitor_hover01,Icon_Monitor_hover02,Icon_Monitor_hover03,
							Icon_Monitor_hover04,Icon_Monitor_hover05,Icon_Monitor_hover06,Icon_Monitor_hover07,
							Icon_Monitor_hover08,Icon_Monitor_hover09,Icon_Monitor_hover10,Icon_Monitor_hover11,
							Icon_Monitor_hover12,Icon_Monitor_hover13,Icon_Monitor_hover14,Icon_Monitor_hover15,
							Icon_Monitor_hover16,Icon_Monitor_hover17
						</sprites>
					</layout>
					<color channel="alpha">
						<clamp>
							<sub>
								<mod>
									<mul>
										<time />
										<static value="15"/>
									</mul>
									<static value="18"/>
								</mod>
								<static value="1" />
							</sub>
							<static value="0" />
							<static value="1" />
						</clamp>
					</color>
					<frameindex frames="18">
						<mul>
							<time />
							<static value="15"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Monitor_pressed00"
			offsetx="2" offsety="2" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_pressed00" blending="premul"/>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_pressed00" x="-7" y="5" blending="premul">
					<layout>
						<sprites>
							Icon_Monitor_pressed00,Icon_Monitor_pressed01,Icon_Monitor_pressed02,Icon_Monitor_pressed03,
							Icon_Monitor_pressed04,Icon_Monitor_pressed05,Icon_Monitor_pressed06,Icon_Monitor_pressed07,
							Icon_Monitor_pressed08,Icon_Monitor_pressed09,Icon_Monitor_pressed10,Icon_Monitor_pressed11,
							Icon_Monitor_pressed12,Icon_Monitor_pressed13,Icon_Monitor_pressed14,Icon_Monitor_pressed15,
							Icon_Monitor_pressed16,Icon_Monitor_pressed17
						</sprites>
					</layout>
					<color channel="alpha">
						<clamp>
							<sub>
								<mod>
									<mul>
										<time />
										<static value="15"/>
									</mul>
									<static value="18"/>
								</mod>
								<static value="1" />
							</sub>
							<static value="0" />
							<static value="1" />
						</clamp>
					</color>
					<frameindex frames="18">
						<mul>
							<time />
							<static value="15"/>
						</mul>
					</frameindex>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="default">
		<mouseRect left="2" top="2" width="306" height="46" />
		<idle spriteSheet="gui_elements" sprite="Button_idle"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="9" />
		<hover spriteSheet="gui_elements" sprite="Button_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="9" />
		<pressed spriteSheet="gui_elements" sprite="Button_pressed"
			offsetx="-6" offsety="-6" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="11" />
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
	<buttonStyle name="selectNextLevel">
		<mouseRect left="-63" top="0" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="right_idle"
			offsetx="4" offsety="35"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="right_hover"
			offsetx="4" offsety="35"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="right_pressed"
			offsetx="-1" offsety="30"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<buttonStyle name="selectPreviousLevel">
		<mouseRect left="-63" top="0" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="left_idle"
			offsetx="4" offsety="35"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="left_hover"
			offsetx="4" offsety="35"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="left_pressed"
			offsetx="-1" offsety="30"
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
	<buttonStyle name="flag_german">
		<mouseRect left="1" top="1" width="64" height="64" />
		<idle spriteSheet="gui_elements" sprite="flag_german"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="9" />
		<hover spriteSheet="gui_elements" sprite="flag_german"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="9" />
		<pressed spriteSheet="gui_elements" sprite="flag_german"
			offsetx="6" offsety="6" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="6" fontoffsety="15" />
	</buttonStyle>
	<buttonStyle name="flag_english">
		<mouseRect left="1" top="1" width="64" height="64" />
		<idle spriteSheet="gui_elements" sprite="flag_english"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="9" />
		<hover spriteSheet="gui_elements" sprite="flag_english"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="9" />
		<pressed spriteSheet="gui_elements" sprite="flag_english"
			offsetx="6" offsety="6" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="6" fontoffsety="15" />
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

	<checkboxStyle name="screenMode">
		<mouseRect left="0" top="0" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_windowed"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_fullscreen"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_windowed_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_fullscreen_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>

	<checkboxStyle name="shaderMode">
		<mouseRect left="0" top="0" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_no_shaders"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_shaders"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_no_shaders_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_shaders_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>

	<checkboxStyle name="shadowMode">
		<mouseRect left="0" top="0" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_no_shadows"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_shadows"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_no_shadows_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_shadows_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>

	<checkboxStyle name="backgroundMusic">
		<mouseRect left="0" top="0" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_background_music"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_no_background_music"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_background_music_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_no_background_music_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>

	<checkboxStyle name="verticalMouse">
		<mouseRect left="10" top="16" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_mouse_horizontal"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_mouse_vertical"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_mouse_horizontal_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_mouse_vertical_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>
	<checkboxStyle name="invertMouse">
		<mouseRect left="10" top="16" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_mouse_no_flip"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_mouse_flip"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_mouse_no_flip_hover"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_mouse_flip_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>
	<checkboxStyle name="language">
		<mouseRect left="10" top="16" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_language_en"
			offsetx="8" offsety="16" />
		<hover spriteSheet="option_elements" sprite="icon_language_en_hover"
			offsetx="8" offsety="16" />
		<checked spriteSheet="option_elements" sprite="icon_language_de"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_language_de_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>
	<checkboxStyle name="batteryStatus">
		<mouseRect left="10" top="16" width="105" height="96" />
		<unchecked spriteSheet="option_elements" sprite="icon_battery_invisible"
			offsetx="8" offsety="15" />
		<hover spriteSheet="option_elements" sprite="icon_battery_invisible_hover"
			offsetx="8" offsety="15" />
		<checked spriteSheet="option_elements" sprite="icon_battery_visible"
			offsetx="8" offsety="16" />
		<checkedHover spriteSheet="option_elements" sprite="icon_battery_visible_hover"
			offsetx="8" offsety="16" />
	</checkboxStyle>
	
	<sliderStyle name="default">
		<mouseRect left="8" top="-3" width="26" height="36" />
		<value min="0" max="100" width="83"/>
		<idle backgroundspriteSheet="gui_elements" backgroundsprite="bar_idle"
			backgroundoffsetx="8" backgroundoffsety="8" 
			sliderspriteSheet="gui_elements" slidersprite="button_idle"
			slideroffsetx="0" slideroffsety="-1" />
		<active backgroundspriteSheet="gui_elements" backgroundsprite="bar_active"
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
		<caret spriteSheet="gui_elements" sprite="caret" srcxoffset="0" srcyoffset="0" width="20" height="40" offsetX="5" offsetY="0" frequency="1"/>
	</inputboxStyle>

	<borderStyle name="default">
		<innerOffsets x="1" y="7" width="-24" height="-15" />
		<background id="1" spriteSheet="gui_elements" sprite="frame_left" height="20" />
		<background id="2" spriteSheet="gui_elements" sprite="frame_top" srcxoffset="1" width="18" />
		<background id="3" spriteSheet="gui_elements" sprite="frame_right" height="20" />
		<background id="4" spriteSheet="gui_elements" sprite="frame_left" srcyoffset="20" height="20" />
		<background id="5" spriteSheet="gui_elements" sprite="frame_right" srcyoffset="20" height="20" />
		<background id="6" spriteSheet="gui_elements" sprite="frame_left" srcyoffset="108" height="11" />
		<background id="7" spriteSheet="gui_elements" sprite="frame_bottom" srcxoffset="1" width="18" />
		<background id="8" spriteSheet="gui_elements" sprite="frame_right" srcyoffset="108" height="11" />
	</borderStyle>
</styles>