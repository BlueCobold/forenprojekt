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
				<vertex x="6" y="54" />
				<vertex x="6" y="60" />
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

			<entity base="small_quad" name="darkwall_1" rep="d1">
				<animations>
					<animation spritesheet="walls" sprite="sand_01" rotate="false">
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
			<entity base="small_quad" name="darkwall_2" rep="d2">
				<animations>
					<animation spritesheet="walls" sprite="sand_02" rotate="false">
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
			<entity base="small_quad" name="darkwall_3" rep="d3">
				<animations>
					<animation spritesheet="walls" sprite="sand_03" rotate="false">
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
			<entity base="small_quad" name="darkwall_4" rep="d4">
				<animations>
					<animation spritesheet="walls" sprite="sand_04" rotate="false">
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
			<entity base="small_quad" name="darkwall_5" rep="d5">
				<animations>
					<animation spritesheet="walls" sprite="sand_05" rotate="false">
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
			<entity base="small_quad" name="darkwall_6" rep="d6">
				<animations>
					<animation spritesheet="walls" sprite="sand_06" rotate="false">
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
					<animation texture="small_quads" srcy="0" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkshadowwall_2" rep="s2">
				<animations>
					<animation texture="small_quads" srcy="60" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkshadowwall_3" rep="s3">
				<animations>
					<animation texture="small_quads" srcy="120" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkshadowwall_4" rep="s4">
				<animations>
					<animation texture="small_quads" srcy="180" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkshadowwall_5" rep="s5">
				<animations>
					<animation texture="small_quads" srcy="240" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkshadowwall_6" rep="s6">
				<animations>
					<animation texture="small_quads" srcy="300" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity base="high_quad" name="high_wall_1" rep="h1">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_2" rep="h2">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_3" rep="h3">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="2"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_4" rep="h4">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="3"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_5" rep="h5">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="4"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="high_quad" name="high_wall_6" rep="h6">
				<animations>
					<animation texture="high_quads" srcy="0" width="60" height="120" rotate="false">
						<frameindex frames="6">
							<static value="5"/>
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity base="wide_quad" name="wide_wall_1" rep="W1">
				<animations>
					<animation texture="wide_quads" srcy="0" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_2" rep="W2">
				<animations>
					<animation texture="wide_quads" srcy="60" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_3" rep="W3">
				<animations>
					<animation texture="wide_quads" srcy="120" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_4" rep="W4">
				<animations>
					<animation texture="wide_quads" srcy="180" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_5" rep="W5">
				<animations>
					<animation texture="wide_quads" srcy="240" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="wide_quad" name="wide_wall_6" rep="W6">
				<animations>
					<animation texture="wide_quads" srcy="300" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity name="wide_darkwall_1" rep="D1">
				<animations>
					<animation texture="wide_quads" srcy="0" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_2" rep="D2">
				<animations>
					<animation texture="wide_quads" srcy="60" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_3" rep="D3">
				<animations>
					<animation texture="wide_quads" srcy="120" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_4" rep="D4">
				<animations>
					<animation texture="wide_quads" srcy="180" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_5" rep="D5">
				<animations>
					<animation texture="wide_quads" srcy="240" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="wide_darkwall_6" rep="D6">
				<animations>
					<animation texture="wide_quads" srcy="300" width="120" height="60" rotate="false">
						<frameindex frames="3">
							<static value="1"/>
						</frameindex>
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
		</entities>
	</templates>
</include>