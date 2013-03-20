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
					<shape type="circle" radius="15" />
				</physics>
				<constants color="0" />
				<animations>
					<animation texture="ball_twirl" width="32" height="32" midx="16" midy="16" rotate="true">
						<frameindex frames="3">
							<var name="color" />
						</frameindex>
					</animation>
					<animation texture="ball_twirl" width="32" height="32" srcx="96" midx="16" midy="16" rotate="false" />
				</animations>
			</entity>
		</entities>
	</templates>
</include>