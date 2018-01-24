<styles>
	<buttonStyle name="achieveButtonStyle">
		<mouseRect left="-50" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Star"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Star" />
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Star_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover_small" copies="11" blending="premul">
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
				<animation spritesheet="gui_animations" sprite="Icon_Star_hover" blending="premul" />
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Star_pressed"
			offsetx="3" offsety="3" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Star_pressed_small" copies="11" blending="premul">
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
				<animation spritesheet="gui_animations" sprite="Icon_Star_pressed" blending="premul" />
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="playButtonStyle">
		<mouseRect left="-50" top="-45" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Play00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Play00" blending="premul" />
				<animation spritesheet="gui_animations" sprite="Icon_Play_button" blending="premul">
					<position axis="y">
						<static value="25" />
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_button_icon" blending="premul">
					<position axis="y">
						<static value="25" />
					</position>
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Play_hover_ball_shadow"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>				
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_ball_shadow" blending="premul" rotate="true">
					<position axis="y">
						<add>
							<static value="-13" />
							<mul>
								<static value="-30" />
								<abs>
									<sine>
										<mul>
											<static value="270" />
											<mod>
												<time />
												<static value="1.333333" />
											</mod>
										</mul>
									</sine>
								</abs>
							</mul>
						</add>
					</position>
					<position axis="x">
						<step threshold="0.666666">
							<setVariable name="t">
								<mod>
									<time />
									<static value="1.333333" />
								</mod>
							</setVariable>
							<add>
								<static value="-25" />
								<mul>
									<static value="75" />
									<variable name="t" />
								</mul>
							</add>
							<add>
								<static value="75" />
								<mul>
									<static value="-75" />
									<variable name="t" />
								</mul>
							</add>
						</step>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_teeter_shadow" blending="premul" rotate="true">
					<position axis="y">
						<static value="17" />
					</position>
					<position axis="x">
						<static value="-1" />
					</position>
					<rotation>
						<mul>
							<static value="25" />
							<sine>
								<mul>
									<static value="270" />
									<mod>
										<time />
										<static value="1.333333" />
									</mod>
								</mul>
							</sine>
						</mul>
					</rotation>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_ball" blending="premul" rotate="true">
					<position axis="y">
						<add>
							<static value="-13" />
							<mul>
								<static value="-30" />
								<abs>
									<sine>
										<mul>
											<static value="270" />
											<mod>
												<time />
												<static value="1.333333" />
											</mod>
										</mul>
									</sine>
								</abs>
							</mul>
						</add>
					</position>
					<position axis="x">
						<step threshold="0.666666">
							<setVariable name="t">
								<mod>
									<time />
									<static value="1.333333" />
								</mod>
							</setVariable>
							<add>
								<static value="-25" />
								<mul>
									<static value="75" />
									<variable name="t" />
								</mul>
							</add>
							<add>
								<static value="75" />
								<mul>
									<static value="-75" />
									<variable name="t" />
								</mul>
							</add>
						</step>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_hover_teeter" blending="premul" rotate="true">
					<position axis="y">
						<static value="17" />
					</position>
					<position axis="x">
						<static value="-1" />
					</position>
					<rotation>
						<mul>
							<static value="25" />
							<sine>
								<mul>
									<static value="270" />
									<mod>
										<time />
										<static value="1.333333" />
									</mod>
								</mul>
							</sine>
						</mul>
					</rotation>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_button_hover" blending="premul">
					<position axis="y">
						<static value="25" />
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_button_icon" blending="premul" rotate="true">
					<position axis="y">
						<static value="25" />
					</position>
					<rotation>
						<step threshold="0.5">
							<mod>
								<time />
								<static value="1.5" />
							</mod>
							<mul>
								<static value="10" />
								<sine>
									<mul>
										<time />
										<static value="1080" />
									</mul>
								</sine>
							</mul>
							<static value="0" />
						</step>
					</rotation>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Play_pressed_ball_glow"
			offsetx="3" offsety="3" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Play_pressed_ball_glow" blending="premul" rotate="true">
					<position axis="y">
						<add>
							<static value="-13" />
							<mul>
								<static value="-30" />
								<abs>
									<sine>
										<mul>
											<static value="270" />
											<mod>
												<time />
												<static value="1.333333" />
											</mod>
										</mul>
									</sine>
								</abs>
							</mul>
						</add>
					</position>
					<position axis="x">
						<step threshold="0.666666">
							<setVariable name="t">
								<mod>
									<time />
									<static value="1.333333" />
								</mod>
							</setVariable>
							<add>
								<static value="-25" />
								<mul>
									<static value="75" />
									<variable name="t" />
								</mul>
							</add>
							<add>
								<static value="75" />
								<mul>
									<static value="-75" />
									<variable name="t" />
								</mul>
							</add>
						</step>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_pressed_teeter_glow" blending="premul" rotate="true">
					<position axis="y">
						<static value="17" />
					</position>
					<position axis="x">
						<static value="-1" />
					</position>
					<rotation>
						<mul>
							<static value="25" />
							<sine>
								<mul>
									<static value="270" />
									<mod>
										<time />
										<static value="1.333333" />
									</mod>
								</mul>
							</sine>
						</mul>
					</rotation>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_pressed_ball" blending="premul" rotate="true">
					<position axis="y">
						<add>
							<static value="-13" />
							<mul>
								<static value="-30" />
								<abs>
									<sine>
										<mul>
											<static value="270" />
											<mod>
												<time />
												<static value="1.333333" />
											</mod>
										</mul>
									</sine>
								</abs>
							</mul>
						</add>
					</position>
					<position axis="x">
						<step threshold="0.666666">
							<setVariable name="t">
								<mod>
									<time />
									<static value="1.333333" />
								</mod>
							</setVariable>
							<add>
								<static value="-25" />
								<mul>
									<static value="75" />
									<variable name="t" />
								</mul>
							</add>
							<add>
								<static value="75" />
								<mul>
									<static value="-75" />
									<variable name="t" />
								</mul>
							</add>
						</step>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_pressed_teeter" blending="premul" rotate="true">
					<position axis="y">
						<static value="17" />
					</position>
					<position axis="x">
						<static value="-1" />
					</position>
					<rotation>
						<mul>
							<static value="25" />
							<sine>
								<mul>
									<static value="270" />
									<mod>
										<time />
										<static value="1.333333" />
									</mod>
								</mul>
							</sine>
						</mul>
					</rotation>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_button_pressed" blending="premul">
					<position axis="y">
						<static value="25" />
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Play_button_icon" blending="premul">
					<position axis="y">
						<static value="25" />
					</position>
					<rotation>
						<step threshold="0.5">
							<mod>
								<time />
								<static value="1.5" />
							</mod>
							<mul>
								<static value="10" />
								<sine>
									<mul>
										<time />
										<static value="1080" />
									</mul>
								</sine>
							</mul>
							<static value="0" />
						</step>
					</rotation>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="replayButtonStyle">
		<mouseRect left="-50" top="-45" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Replay_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Replay_idle" blending="premul" rotate="true">
				</animation>
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Replay_hover"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Replay_hover" blending="premul" rotate="true" offsetx="5">
					<rotation>
						<sine amplitude="20" period="250">
							<time />
						</sine>
					</rotation>
				</animation>
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Replay_pressed"
			offsetx="3" offsety="3" useOrigin="true" sound="bounce" 
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Replay_pressed" blending="premul" rotate="true" offsetx="5">
					<rotation>
						<sine amplitude="20" period="250">
							<time />
						</sine>
					</rotation>
				</animation>
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="optionButtonStyle">
		<mouseRect left="-50" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Wheels00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Wheels00" blending="premul" />
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
			offsetx="3" offsety="3" sound="bounce" 
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
		<mouseRect left="-50" top="-50" width="100" height="100" />
		<idle spriteSheet="gui_animations" sprite="Icon_Door00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Door00" blending="premul" />
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Door_hover_arrow_shadow"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover_arrow_shadow" blending="premul">
					<position axis="x">
						<abs>
							<mul>
								<static value="20" />
								<sine>
									<mul>
										<time />
										<static value="250" />
									</mul>
								</sine>
							</mul>
						</abs>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover_shadow" blending="premul" />
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover" blending="premul" />
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover_arrow" blending="premul">
					<position axis="x">
						<abs>
							<mul>
								<static value="20" />
								<sine>
									<mul>
										<time />
										<static value="250" />
									</mul>
								</sine>
							</mul>
						</abs>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Door_hover_part" blending="premul" />
			</animations>
		</hover>
		<pressed spriteSheet="gui_animations" sprite="Icon_Door_pressed_arrow_glow"
			offsetx="3" offsety="3" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed_arrow_glow" blending="premul">
					<position axis="x">
						<abs>
							<mul>
								<static value="20" />
								<sine>
									<mul>
										<time />
										<static value="250" />
									</mul>
								</sine>
							</mul>
						</abs>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed_glow" blending="premul" />
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed" blending="premul" />
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed_arrow" blending="premul">
					<position axis="x">
						<abs>
							<mul>
								<static value="20" />
								<sine>
									<mul>
										<time />
										<static value="250" />
									</mul>
								</sine>
							</mul>
						</abs>
					</position>
				</animation>
				<animation spritesheet="gui_animations" sprite="Icon_Door_pressed_part" blending="premul" />
			</animations>
		</pressed>
	</buttonStyle>
	<buttonStyle name="creditButtonStyle">
		<mouseRect left="-50" top="-45" width="100" height="95" />
		<idle spriteSheet="gui_animations" sprite="Icon_Monitor00"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" >
			<animations resetOnExit="true">
				<animation spritesheet="gui_animations" sprite="Icon_Monitor00" blending="premul" />
			</animations>
		</idle>
		<hover spriteSheet="gui_animations" sprite="Icon_Monitor_hover00"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_hover00" blending="premul" />
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
			offsetx="3" offsety="3" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="5">
			<animations>
				<animation spritesheet="gui_animations" sprite="Icon_Monitor_pressed00" blending="premul" />
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
		<mouseRect left="-153" top="-23" width="306" height="46" />
		<idle spriteSheet="gui_elements" sprite="Button_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="-19" />
		<hover spriteSheet="gui_elements" sprite="Button_hover"
			offsetx="0" offsety="0" sound="bounce" useOrigin="true"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="-19" />
		<pressed spriteSheet="gui_elements" sprite="Button_pressed"
			offsetx="-2" offsety="-2" sound="bounce" useOrigin="true"
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="-17" />
	</buttonStyle>
	<buttonStyle name="square">
		<mouseRect left="-18" top="-18" width="36" height="36" />
		<idle spriteSheet="gui_elements" sprite="normal"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="-2" fontoffsety="-19" />
		<hover spriteSheet="gui_elements" sprite="hover"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce"
			font="buttonHoverFont" fontoffsetx="-2" fontoffsety="-19" />
		<pressed spriteSheet="gui_elements" sprite="pressed"
			offsetx="-2" offsety="-2" useOrigin="true" sound="bounce"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="-17" />
	</buttonStyle>
	<buttonStyle name="coins">
		<mouseRect left="-57" top="-47" width="114" height="94" />
		<idle spriteSheet="gui_animations" sprite="Icon_Coins_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_animations" sprite="Icon_Coins_hover"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_animations" sprite="Icon_Coins_pressed"
			offsetx="3" offsety="3" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
	</buttonStyle>
	<buttonStyle name="goblet">
		<mouseRect left="-53" top="-45" width="106" height="90" />
		<idle spriteSheet="gui_animations" sprite="Goblet_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<hover spriteSheet="gui_animations" sprite="Goblet_hover"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="gui_animations" sprite="Goblet_pressed"
			offsetx="3" offsety="3" useOrigin="true" sound="bounce" 
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="4" />
	</buttonStyle>
	<buttonStyle name="small">
		<mouseRect left="-70" top="-23" width="141" height="46" />
		<idle spriteSheet="gui_elements" sprite="Button_small_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="-19" />
		<hover spriteSheet="gui_elements" sprite="Button_small_hover"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="-19" />
		<pressed spriteSheet="gui_elements" sprite="Button_small_pressed"
			offsetx="-2" offsety="-2" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="1" fontoffsety="-17" />
	</buttonStyle>
	<buttonStyle name="selectLevel">
		<mouseRect left="-96" top="-54" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="missing"
			offsetx="0" offsety="0" useOrigin="true" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<buttonStyle name="selectNextLevel">
		<mouseRect left="-96" top="-54" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="right_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="right_hover"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="right_pressed"
			offsetx="-2" offsety="-2" useOrigin="true"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
	</buttonStyle>
	<buttonStyle name="selectPreviousLevel">
		<mouseRect left="-96" top="-54" width="192" height="108" />
		<idle spriteSheet="gui_elements" sprite="left_idle"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="gui_elements" sprite="left_hover"
			offsetx="0" offsety="0" useOrigin="true"
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="0" />
		<pressed spriteSheet="gui_elements" sprite="left_pressed"
			offsetx="-2" offsety="-2" useOrigin="true"
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
	<buttonStyle name="optionBackScreen">
		<mouseRect left="0" top="0" width="105" height="96" />
		<idle spriteSheet="option_elements" sprite="icon_graphics_background"
			offsetx="0" offsety="0"
			font="buttonIdleFont" fontoffsetx="0" fontoffsety="0" />
		<hover spriteSheet="option_elements" sprite="icon_graphics_background_hover"
			offsetx="0" offsety="0" sound="bounce" 
			font="buttonHoverFont" fontoffsetx="0" fontoffsety="4" />
		<pressed spriteSheet="option_elements" sprite="icon_graphics_background_hover"
			offsetx="0" offsety="0" sound="bounce" 
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

	<checkboxStyle name="highscoreMode">
		<mouseRect left="-48" top="-48" width="96" height="96" />
		<unchecked spriteSheet="gui_animations" sprite="Icon_Highscore_Local_idle"
			offsetx="0" offsety="0" useOrigin="true" />
		<checked spriteSheet="gui_animations" sprite="Icon_Highscore_Global_idle"
			offsetx="0" offsety="0" useOrigin="true" />
		<hover spriteSheet="gui_animations" sprite="Icon_Highscore_Local_hover"
			offsetx="0" offsety="0" useOrigin="true" />
		<checkedHover spriteSheet="gui_animations" sprite="Icon_Highscore_Global_hover"
			offsetx="0" offsety="0" useOrigin="true" />
	</checkboxStyle>

	<checkboxStyle name="gameMode">
		<mouseRect left="0" top="0" width="85" height="96" />
		<unchecked spriteSheet="gui_animations" sprite="Icons_Targets_idle"
			offsetx="0" offsety="0" />
		<checked spriteSheet="gui_animations" sprite="Icon_Clock_idle"
			offsetx="0" offsety="0" />
		<hover spriteSheet="gui_animations" sprite="Icons_Targets_hover"
			offsetx="0" offsety="0" />
		<checkedHover spriteSheet="gui_animations" sprite="Icon_Clock_hover"
			offsetx="0" offsety="0" />
	</checkboxStyle>
	<checkboxStyle name="gameModeCentered">
		<mouseRect left="-48" top="-48" width="90" height="96" />
		<unchecked spriteSheet="gui_animations" sprite="Icons_Targets_idle"
			offsetx="0" offsety="0" useOrigin="true" />
		<checked spriteSheet="gui_animations" sprite="Icon_Clock_idle"
			offsetx="5" offsety="5" useOrigin="true" />
		<hover spriteSheet="gui_animations" sprite="Icons_Targets_hover"
			offsetx="0" offsety="0" useOrigin="true" />
		<checkedHover spriteSheet="gui_animations" sprite="Icon_Clock_hover"
			offsetx="5" offsety="5" useOrigin="true" />
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

	<borderStyle name="window" color="D2D2D2D2">
			<background id="1" spriteSheet="gui_elements" sprite="window_left_upper" />
			<background id="2" spriteSheet="gui_elements" sprite="window_top" widthoffset="-2" srcxoffset="1" />
			<background id="3" spriteSheet="gui_elements" sprite="window_right_upper" />
			<background id="4" spriteSheet="gui_elements" sprite="window_left" heightoffset="-2" srcyoffset="1" />
			<background id="5" spriteSheet="gui_elements" sprite="window_right" heightoffset="-2" srcyoffset="1" />
			<background id="6" spriteSheet="gui_elements" sprite="window_left_lower" />
			<background id="7" spriteSheet="gui_elements" sprite="window_bottom" widthoffset="-2" srcxoffset="1" />
			<background id="8" spriteSheet="gui_elements" sprite="window_right_lower" />
			<background id="9" spriteSheet="gui_elements" sprite="window_main" widthoffset="-2" srcxoffset="1" heightoffset="-2" srcyoffset="1" />
			<deco id="2" spriteSheet="gui_elements" sprite="window_deco_top" offsetx="-200" offsety="-36"/>
			<deco id="2" spriteSheet="gui_elements" sprite="window_deco_top_right" offsetx="-24" offsety="0"/>
			<deco id="2" spriteSheet="gui_elements" sprite="window_deco_right" offsetx="-1" offsety="-30" />
			<deco id="3" spriteSheet="gui_elements" sprite="window_deco_left" offsetx="-43" offsety="-157" />
			<deco id="3" spriteSheet="gui_elements" sprite="window_deco_bottom_left" offsetx="0" offsety="-24" />
			<deco id="3" spriteSheet="gui_elements" sprite="window_deco_bottom" offsetx="2" offsety="-1" />
	</borderStyle>
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