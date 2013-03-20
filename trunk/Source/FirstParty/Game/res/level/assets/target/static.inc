<include>
	<templates>
		<shapes>
			<shape name="teeter" type="polygon">
				<vertex x="-104" y="-15" />
				<vertex x="104" y="-15" />
				<vertex x="104" y="15" />
				<vertex x="-104" y="15" />
			</shape>
			<shape name="teeter2" type="polygon">
				<vertex x="-15" y="-104" />
				<vertex x="-15" y="104" />
				<vertex x="15" y="104" />
				<vertex x="15" y="-104" />
			</shape>
		</shapes>
		
		<functions>
			<function name="target_bloat">
				<step>
					<add>
						<time />
						<static value="0.75"/>
					</add>
					<add>
						<static value="1" />
						<mul>
							<sine>
								<mul>
									<time />
									<static value="360" />
								</mul>
							</sine>
							<static value="0.25" />
						</mul>
					</add>
					<step>
						<add>
							<time />
							<static value="0.5"/>
						</add>
						<mul>
							<sine>
								<mul>
									<time />
									<static value="360" />
								</mul>
							</sine>
							<static value="1.25" />
						</mul>
						<stop />
					</step>
				</step>
			</function>
		</functions>

		<physics>
			<physic name="target">
				<body type="kinematic" angle="0" />
				<fixture density="0.5" friction="0.8" restitution="0.1" />
			</physic>
		</physics>
		
		<entities>
			<entity name="target_2" rep="--" collideWithBall="false">
				<physics name="target">
					<shape type="circle" radius="35" />
				</physics>
				<animations>
					<animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true">
						<scale axis="x">
							<function name="target_bloat" />
						</scale>
						<scale axis="y">
							<function name="target_bloat" />
						</scale>
					</animation>
				</animations>
			</entity>
			
			<entity name="target_1" rep="t1" type="target" collideWithBall="false">
				<physics name="target">
					<shape type="circle" radius="30" />
				</physics>
				<collides>
					<spawnEntity name="target_2">
						<always />
					</spawnEntity>
				</collides>
				<animations>
					<animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true" />
				</animations>
			</entity>
		</entities>
	</templates>
</include>