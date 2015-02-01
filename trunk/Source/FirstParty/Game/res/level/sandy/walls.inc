<include>
	<templates>
		<shapes>
			<shape name="wall" type="polygon">
				<vertex x="-30" y="-30" />
				<vertex x="30" y="-30" />
				<vertex x="30" y="30" />
				<vertex x="-30" y="30" />
			</shape>
			<shape name="high_wall" type="polygon">
				<vertex x="1" y="1" />
				<vertex x="59" y="1" />
				<vertex x="59" y="119" />
				<vertex x="1" y="119" />
			</shape>
			<shape name="wide_wall" type="polygon">
				<vertex x="0" y="0" />
				<vertex x="120" y="0" />
				<vertex x="120" y="60" />
				<vertex x="0" y="60" />
			</shape>
			<shape name="left_lower" type="polygon">
				<vertex x="0" y="6" />
				<vertex x="6" y="6" />
				<vertex x="54" y="54" />
				<vertex x="54" y="60" />
				<vertex x="0" y="60" />
			</shape>
			<shape name="left_upper" type="polygon">
				<vertex x="0" y="0" />
				<vertex x="54" y="0" />
				<vertex x="54" y="6" />
				<vertex x="6" y="54" />
				<vertex x="0" y="54" />
			</shape>
			<shape name="right_lower" type="polygon">
				<vertex x="54" y="6" />
				<vertex x="60" y="6" />
				<vertex x="60" y="60" />
				<vertex x="6" y="60" />
				<vertex x="6" y="54" />
			</shape>
			<shape name="right_upper" type="polygon">
				<vertex x="6" y="0" />
				<vertex x="60" y="0" />
				<vertex x="60" y="54" />
				<vertex x="54" y="54" />
				<vertex x="6" y="6" />
			</shape>
		</shapes>

		<physics>
			<physic name="wall">
				<body type="static" angle="0" />
				<fixture density="0.1" friction="0.8" restitution="0.5" />
			</physic>
		</physics>

		<entities>
			<entity respawnable="true" stopWithLastAnimation="true" name="wall_hit_particles" rep="--" collideWithBall="false">
				<constants scale="1" xoffset="0" yoffset="0" />
				<animations>
					<animation copies="7" texture="sparkles" srcx="256" width="32" height="32" midx="16" midy="16" rotate="true">
						<constants size="1" time="0"/>
						<frameindex frames="8">
							<step>
								<setVar name="time">
									<clamp>
										<mul>
											<time />
											<cache>
												<random min="0.9" max="1.3" />
											</cache>
										</mul>
										<static value="0" />
										<static value="1" />
									</clamp>
								</setVar>
								<cache>
									<int>
										<random min="0" max="8" />
									</int>
								</cache>
								<stopAnimation />
							</step>
						</frameindex>
						<position axis="x">
							<mul>
								<var name="time" />
								<cache>
									<random min="-20" max="20" />
								</cache>
							</mul>
						</position>
						<position axis="y">
							<mul>
								<var name="time" />
								<cache>
									<random min="-20" max="20" />
								</cache>
							</mul>
						</position>
						<scale axis="x">
							<cache>
								<setVar name="size">
									<mul>
										<random min="0.2" max="0.7" />
										<var name="scale"/>
									</mul>
								</setVar>
							</cache>
						</scale>
						<scale axis="y">
							<cache>
								<var name="size" />
							</cache>
						</scale>

						<color channel="red">
							<cache>
								<random min="0.40" max="0.55" />
							</cache>
						</color>
						<color channel="green">
							<cache>
								<random min="0.30" max="0.45" />
							</cache>
						</color>
						<color channel="blue">
							<cache>
								<random min="0.05" max="0.20" />
							</cache>
						</color>
						<color channel="alpha">
							<clamp>
								<sub>
									<static value="1" />
									<var name="time" />
								</sub>
								<static value="0" />
								<static value="1" />
							</clamp>
						</color>
					</animation>
				</animations>
			</entity>

			<entity name="base_wall" rep="bw">
				<onCollision>
					<spawnEntity name="wall_hit_particles" location="contact" />
				</onCollision>
			</entity>
			<entity base="base_wall" name="small_quad" rep="sq" offsetx="30" offsety="30">
				<physics name="wall" shape="wall" />
			</entity>
			<entity base="base_wall" name="high_quad" rep="hq">
				<physics name="wall" shape="high_wall" />
			</entity>
			<entity base="base_wall" name="wide_quad" rep="wq">
				<physics name="wall" shape="wide_wall" />
			</entity>
			<entity base="base_wall" name="lu_base" rep="lu">
				<physics name="wall" shape="left_upper" />
			</entity>
			<entity base="base_wall" name="ll_base" rep="ll">
				<physics name="wall" shape="left_lower" />
			</entity>
			<entity base="base_wall" name="ru_base" rep="ru">
				<physics name="wall" shape="right_upper" />
			</entity>
			<entity base="base_wall" name="rl_base" rep="rl">
				<physics name="wall" shape="right_lower" />
			</entity>

			<entity name="gras_half_left" rep="ga" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_left" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_half_right" rep="gb" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_right" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_left" rep="gc" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_left" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_middle_left" rep="gd" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_left" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_middle_right" rep="ge" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_right" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_right" rep="gf" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_right" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_half_left_45" rep="gg" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_left_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_half_right_45" rep="gh" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_right_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_left_45" rep="gi" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_left_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_middle_left_45" rep="gj" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_left_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_middle_right_45" rep="gk" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_right_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_right_45" rep="gl" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_right_45" rotate="false"/>
				</animations>
			</entity>
			<entity name="gras_half_left_45_mirror" rep="gm" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_left_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_half_right_45_mirror" rep="gn" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_half_right_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_left_45_mirror" rep="go" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_left_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_middle_left_45_mirror" rep="gp" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_left_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_middle_right_45_mirror" rep="gq" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_middle_right_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_right_45_mirror" rep="gr" z="-1" offsetx="60">
				<animations>
					<animation spritesheet="walls" sprite="gras_right_45" rotate="false">
						<scale axis="x">
							<static value="-1" />
						</scale>
					</animation>
				</animations>
			</entity>
			<entity name="gras_one_tile" rep="gs" z="-1">
				<animations>
					<animation spritesheet="walls" sprite="gras_left"  x="-30" rotate="false"/>
					<animation spritesheet="walls" sprite="gras_right" x="30"rotate="false"/>
				</animations>
			</entity>
			
			<entity base="small_quad" name="wall_1" rep="w1">
				<animations>
					<animation spritesheet="walls" sprite="sand_01" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_2" rep="w2">
				<animations>
					<animation spritesheet="walls" sprite="sand_02" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_3" rep="w3">
				<animations>
					<animation spritesheet="walls" sprite="sand_03" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_4" rep="w4">
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_5" rep="w5">
				<animations>
					<animation spritesheet="walls" sprite="sand_05" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_6" rep="w6">
				<animations>
					<animation spritesheet="walls" sprite="sand_06" rotate="false"/>
				</animations>
			</entity>

			<entity name="darkwall_1" rep="d1">
				<animations>
					<animation spritesheet="walls" sprite="sand_01" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_2" rep="d2">
				<animations>
					<animation spritesheet="walls" sprite="sand_02" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_3" rep="d3">
				<animations>
					<animation spritesheet="walls" sprite="sand_03" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_4" rep="d4">
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_5" rep="d5">
				<animations>
					<animation spritesheet="walls" sprite="sand_05" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_6" rep="d6">
				<animations>
					<animation spritesheet="walls" sprite="sand_06" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>

			<entity name="darkshadowwall_1" rep="s1">
				<animations>
					<animation spritesheet="walls" sprite="sand_01" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>
			<entity name="darkshadowwall_2" rep="s2">
				<animations>
					<animation spritesheet="walls" sprite="sand_02" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>
			<entity name="darkshadowwall_3" rep="s3">
				<animations>
					<animation spritesheet="walls" sprite="sand_03" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>
			<entity name="darkshadowwall_4" rep="s4">
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>
			<entity name="darkshadowwall_5" rep="s5">
				<animations>
					<animation spritesheet="walls" sprite="sand_05" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>
			<entity name="darkshadowwall_6" rep="s6">
				<animations>
					<animation spritesheet="walls" sprite="sand_06" rotate="false" x="30" y="30">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
					<animation spritesheet="walls" sprite="shadow" rotate="false"/>
				</animations>
			</entity>


			<entity base="high_quad" name="high_wall_1" rep="h1">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_01" rotate="false" />
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_2" rep="h2">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_02" rotate="false" />
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_3" rep="h3">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_03" rotate="false" />
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_4" rep="h4">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_04" rotate="false" />
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_5" rep="h5">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_05" rotate="false" />
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_6" rep="h6">
				<animations>
					<animation spritesheet="walls" sprite="sand_high_06" rotate="false" />
				</animations>
			</entity>


			<entity base="wide_quad" name="wide_wall_1" rep="W1">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_01" rotate="false" />
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_2" rep="W2">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_02" rotate="false" />
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_3" rep="W3">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_03" rotate="false" />
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_4" rep="W4">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_04" rotate="false" />
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_5" rep="W5">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_05" rotate="false" />
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_6" rep="W6">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_06" rotate="false" />
				</animations>
			</entity>


			<entity name="wide_darkwall_1" rep="D1">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_01" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_2" rep="D2">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_02" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_3" rep="D3">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_03" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_4" rep="D4">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_04" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_5" rep="D5">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_05" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_6" rep="D6">
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_06" rotate="false">
						<color channel="red">
							<static value="0.8"/>
						</color>
						<color channel="green">
							<static value="0.7"/>
						</color>
						<color channel="blue">
							<static value="0.7"/>
						</color>
					</animation>
				</animations>
			</entity>


			<entity base="lu_base" name="left_upper" rep=")1">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")2">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")3">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")4">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")5">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")6">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false" />
				</animations>
			</entity>


			<entity base="ll_base" name="left_lower" rep="\1">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\2">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\3">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\4">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\5">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\6">
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false" />
				</animations>
			</entity>


			<entity base="ru_base" name="right_upper" rep="(1">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(2">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(3">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(4">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(5">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(6">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false" />
				</animations>
			</entity>

			<entity base="rl_base" name="right_lower" rep="/1">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/2">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/3">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/4">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/5">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/6">
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false" />
				</animations>
			</entity>
			
			
			<entity base="small_quad" name="wood_wall_1" rep="q1">
				<animations>
					<animation spritesheet="walls" sprite="wood_01" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wood_wall_2" rep="q2">
				<animations>
					<animation spritesheet="walls" sprite="wood_02" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wood_wall_3" rep="q3">
				<animations>
					<animation spritesheet="walls" sprite="wood_03" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="wood_wall_4" rep="q4">
				<animations>
					<animation spritesheet="walls" sprite="wood_04" rotate="false"/>
				</animations>
			</entity>
			
			
			<entity name="window_wall_1" rep="g1">
				<animations>
					<animation spritesheet="walls" sprite="window_01" rotate="false"/>
				</animations>
			</entity>
			<entity name="window_wall_2" rep="g2">
				<animations>
					<animation spritesheet="walls" sprite="window_02" rotate="false"/>
				</animations>
			</entity>
			<entity name="window_wall_3" rep="g3">
				<animations>
					<animation spritesheet="walls" sprite="window_03" rotate="false"/>
				</animations>
			</entity>
			<entity name="window_wall_4" rep="g4">
				<animations>
					<animation spritesheet="walls" sprite="window_04" rotate="false"/>
				</animations>
			</entity>
			
			<entity base="small_quad" name="metalwall_1" rep="M1">
				<animations>
					<animation spritesheet="walls" sprite="metal_01" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="metalwall_2" rep="M2">
				<animations>
					<animation spritesheet="walls" sprite="metal_02" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="metalwall_3" rep="M3">
				<animations>
					<animation spritesheet="walls" sprite="metal_03" rotate="false"/>
				</animations>
			</entity>
			<entity base="small_quad" name="metalwall_4" rep="M4">
				<animations>
					<animation spritesheet="walls" sprite="metal_04" rotate="false"/>
				</animations>
			</entity>
			<entity name="red_colored_wall" rep="X1" offsetx="30" offsety="30">
				<physics name="wall" shape="wall" />
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="0" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
						<color value="FF6060FF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_lower" name="red_colored_right_lower" rep="/r">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="0" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
						<color value="FF6060FF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_lower" name="red_colored_left_lower" rep="\r">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="0" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
						<color value="FF6060FF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_upper" name="red_colored_right_upper" rep="(r">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="0" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
						<color value="FF6060FF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_upper" name="red_colored_left_upper" rep=")r">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="0" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
						<color value="FF6060FF" />
					</animation>
				</animations>
			</entity>
			<entity name="green_colored_wall" rep="X2" offsetx="30" offsety="30">
				<physics name="wall" shape="wall" />
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="1" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false">
						<frameindex frames="5">
							<static value="0"/>
						</frameindex>
						<color value="60FF60FF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_lower" name="red_colored_right_lower" rep="/g">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="1" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false">
						<frameindex frames="5">
							<static value="0"/>
						</frameindex>
						<color value="60FF60FF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_lower" name="green_colored_left_lower" rep="\g">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="1" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false">
						<frameindex frames="5">
							<static value="0"/>
						</frameindex>
						<color value="60FF60FF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_upper" name="green_colored_right_upper" rep="(g">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="1" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false">
						<frameindex frames="5">
							<static value="0"/>
						</frameindex>
						<color value="60FF60FF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_upper" name="green_colored_left_upper" rep=")g">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="1" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false">
						<frameindex frames="5">
							<static value="0"/>
						</frameindex>
						<color value="60FF60FF" />
					</animation>
				</animations>
			</entity>
			<entity name="blue_colored_wall" rep="X3" offsetx="30" offsety="30">
				<physics name="wall" shape="wall" />
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="2" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
						<color value="6060FFFF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_lower" name="blue_colored_right_lower" rep="/b">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="2" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_lower" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
						<color value="6060FFFF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_lower" name="blue_colored_left_lower" rep="\b">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="2" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_lower" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
						<color value="6060FFFF" />
					</animation>
				</animations>
			</entity>
			<entity base="right_upper" name="blue_colored_right_upper" rep="(b">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="2" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_right_upper" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
						<color value="6060FFFF" />
					</animation>
				</animations>
			</entity>
			<entity base="left_upper" name="blue_colored_left_upper" rep=")b">
				<sound name="refuse" />
				<collides>
					<propertyFilter target="ball">
						<abs>
							<sub>
								<static value="2" />
								<var name="ball:color" />
							</sub>
						</abs>
					</propertyFilter>
				</collides>
				<onCollision>
					<showLabel font="red" offset="50">
						<label>levelBlocked1</label>
						<label>levelBlocked2</label>
						<label>levelBlocked3</label>
					</showLabel>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_left_upper" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
						<color value="6060FFFF" />
					</animation>
				</animations>
			</entity>
			<entity name="change_wall" rep="x1">
				<physics name="wall" shape="high_wall" />
				<sound name="switch" />
				<onCollision>
					<changeProperty name="ball:color">
						<mod>
							<add>
								<var name="ball:color" />
								<static value="1" />
							</add>
							<static value="3" />
						</mod>
					</changeProperty>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_high_01" rotate="false" >
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
						<color channel="red">
							<mul>
								<add>
									<static value="1" />
									<sine>
										<mul>
											<static value="150" />
											<time />
										</mul>
									</sine>
								</add>
								<static value="0.5" />
							</mul>
						</color>
						<color channel="blue">
							<mul>
								<add>
									<static value="1" />
									<sine>
										<mul>
											<static value="130" />
											<add>
												<time />
												<static value="180" />
											</add>
										</mul>
									</sine>
								</add>
								<static value="0.5" />
							</mul>
						</color>
					</animation>
				</animations>
			</entity>
			<entity name="change_wall" rep="x2">
				<physics name="wall" shape="wide_wall" />
				<sound name="switch" />
				<onCollision>
					<changeProperty name="ball:color">
						<mod>
							<add>
								<var name="ball:color" />
								<static value="1" />
							</add>
							<static value="3" />
						</mod>
					</changeProperty>
				</onCollision>
				<animations>
					<animation spritesheet="walls" sprite="sand_wide_01" rotate="false" >
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
						<color channel="red">
							<mul>
								<add>
									<static value="1" />
									<sine>
										<mul>
											<static value="150" />
											<time />
										</mul>
									</sine>
								</add>
								<static value="0.5" />
							</mul>
						</color>
						<color channel="blue">
							<mul>
								<add>
									<static value="1" />
									<sine>
										<mul>
											<static value="130" />
											<add>
												<time />
												<static value="180" />
											</add>
										</mul>
									</sine>
								</add>
								<static value="0.5" />
							</mul>
						</color>
					</animation>
				</animations>
			</entity>
		</entities>
	</templates>
</include>