<include>
	<templates>
		<physics>
			<physic name="ball">
				<body type="dynamic" angle="0" angularDamping="4.0" />
				<fixture density="0.5" friction="0.7" restitution="0.4" />
			</physic>
		</physics>

		<entities>
			<!-- Ball -->
			<entity name="Ball" rep="BB" type="ball">
				<physics name="ball">
					<shape type="circle" radius="14" />
				</physics>
				<constants color="0" />
				<animations>
					<animation texture="ballback" width="30" height="30" midx="15" midy="15" rotate="true">
						<frameindex frames="3">
							<var name="color" />
						</frameindex>
					</animation>
					<animation texture="ballshade" width="30" height="30" midx="15" midy="15" rotate="false" />
				</animations>
			</entity>
		</entities>
	</templates>
</include>