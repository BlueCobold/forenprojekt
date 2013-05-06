<include>
	<templates>
		<shapes>
			<shape name="wall" type="polygon">
				<vertex x="1" y="1" />
				<vertex x="59" y="1" />
				<vertex x="59" y="59" />
				<vertex x="1" y="59" />
			</shape>
			<shape name="high_wall" type="polygon">
				<vertex x="1" y="1" />
				<vertex x="59" y="1" />
				<vertex x="59" y="119" />
				<vertex x="1" y="119" />
			</shape>
			<shape name="wide_wall" type="polygon">
				<vertex x="1" y="1" />
				<vertex x="119" y="1" />
				<vertex x="119" y="59" />
				<vertex x="1" y="59" />
			</shape>
			<shape name="left_lower" type="polygon">
				<vertex x="1" y="6" />
				<vertex x="6" y="6" />
				<vertex x="54" y="54" />
				<vertex x="54" y="59" />
				<vertex x="1" y="59" />
			</shape>
			<shape name="left_upper" type="polygon">
				<vertex x="1" y="1" />
				<vertex x="54" y="1" />
				<vertex x="54" y="6" />
				<vertex x="6" y="54" />
				<vertex x="1" y="54" />
			</shape>
			<shape name="right_lower" type="polygon">
				<vertex x="54" y="6" />
				<vertex x="59" y="6" />
				<vertex x="59" y="59" />
				<vertex x="6" y="54" />
				<vertex x="6" y="59" />
			</shape>
			<shape name="right_upper" type="polygon">
				<vertex x="6" y="1" />
				<vertex x="59" y="1" />
				<vertex x="59" y="54" />
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
			<entity name="base_wall" rep="bw">
			</entity>
			<entity base="base_wall" name="small_quad" rep="sq">
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
					<animation texture="small_quads" srcy="0" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_2" rep="w2">
				<animations>
					<animation texture="small_quads" srcy="60" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_3" rep="w3">
				<animations>
					<animation texture="small_quads" srcy="120" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_4" rep="w4">
				<animations>
					<animation texture="small_quads" srcy="180" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_5" rep="w5">
				<animations>
					<animation texture="small_quads" srcy="240" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="small_quad" name="wall_6" rep="w6">
				<animations>
					<animation texture="small_quads" srcy="300" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="0"/>
						</frameindex>
					</animation>
				</animations>
			</entity>

			<entity name="darkwall_1" rep="d1">
				<animations>
					<animation texture="small_quads" srcy="0" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_2" rep="d2">
				<animations>
					<animation texture="small_quads" srcy="60" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_3" rep="d3">
				<animations>
					<animation texture="small_quads" srcy="120" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_4" rep="d4">
				<animations>
					<animation texture="small_quads" srcy="180" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_5" rep="d5">
				<animations>
					<animation texture="small_quads" srcy="240" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="darkwall_6" rep="d6">
				<animations>
					<animation texture="small_quads" srcy="300" width="60" height="60" rotate="false">
						<frameindex frames="4">
							<static value="1"/>
						</frameindex>
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
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false" />
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")2">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false">
						<frameindex frames="6">
							<static value="1" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")3">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false">
						<frameindex frames="6">
							<static value="2" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")4">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false">
						<frameindex frames="6">
							<static value="3" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")5">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false">
						<frameindex frames="6">
							<static value="4" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="lu_base" name="left_upper" rep=")6">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="120" rotate="false">
						<frameindex frames="6">
							<static value="5" />
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity base="ll_base" name="left_lower" rep="\1">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false" />
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\2">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false">
						<frameindex frames="6">
							<static value="1" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\3">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false">
						<frameindex frames="6">
							<static value="2" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\4">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false">
						<frameindex frames="6">
							<static value="3" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\5">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false">
						<frameindex frames="6">
							<static value="4" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ll_base" name="left_lower" rep="\6">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="60" rotate="false">
						<frameindex frames="6">
							<static value="5" />
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity base="ru_base" name="right_upper" rep="(1">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false" />
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(2">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false">
						<frameindex frames="6">
							<static value="1" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(3">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false">
						<frameindex frames="6">
							<static value="2" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(4">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false">
						<frameindex frames="6">
							<static value="3" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(5">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false">
						<frameindex frames="6">
							<static value="4" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="ru_base" name="right_upper" rep="(6">
				<animations>
					<animation texture="small_triangles" width="60" height="60" srcy="180" rotate="false">
						<frameindex frames="6">
							<static value="5" />
						</frameindex>
					</animation>
				</animations>
			</entity>


			<entity base="rl_base" name="right_lower" rep="/1">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false" />
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/2">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false">
						<frameindex frames="6">
							<static value="1" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/3">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false">
						<frameindex frames="6">
							<static value="2" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/4">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false">
						<frameindex frames="6">
							<static value="3" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/5">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false">
						<frameindex frames="6">
							<static value="4" />
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity base="rl_base" name="right_lower" rep="/6">
				<animations>
					<animation texture="small_triangles" width="60" height="60" rotate="false">
						<frameindex frames="6">
							<static value="5" />
						</frameindex>
					</animation>
				</animations>
			</entity>
		</entities>
	</templates>
</include>