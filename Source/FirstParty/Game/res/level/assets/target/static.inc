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
			<entity name="target_particles" rep="--" collideWithBall="false">
				<animations>
					<animation copies="20" texture="sparkles" width="32" height="32" midx="16" midy="16" rotate="true">
						<constants posx="0" posy="0" shape="0" init="0" time="0" size="1"/>
						<position axis="x">
							<step>
								<add>
									<setVar name="time">
										<sub>
											<time />
											<static value="0.40" />
										</sub>
									</setVar>
									<static value="1" />
								</add>
								<static value="0" />
								<mul>
									<pow>
										<max>
											<var name="time" />
											<static value="0" />
										</max>
										<static value="0.4" />
									</pow>
									<step>
										<var name="init" />
										<nop>
											<setVar name="posx">
												<random min="-80" max="80" />
											</setVar>
											<setVar name="posy">
												<random min="-80" max="80" />
											</setVar>
											<setVar name="size">
												<random min="0.3" max="1.0" />
											</setVar>
											<setVar name="shape">
												<int>
													<random min="0" max="8" />
												</int>
											</setVar>
											<setVar name="init">
												<static value="1" />
											</setVar>
										</nop>
										<var name="posx" />
									</step>
								</mul>
							</step>
						</position>
						<position axis="y">
							<mul>
								<pow>
									<max>
										<var name="time" />
										<static value="0" />
									</max>
									<static value="0.4" />
								</pow>
								<var name="posy" />
							</mul>
						</position>
						<scale axis="x">
							<var name="size" />
						</scale>
						<scale axis="y">
							<var name="size" />
						</scale>
						<frameindex frames="8">
							<step>
								<var name="time" />
								<var name="shape" />
								<stop />
							</step>
						</frameindex>
					</animation>
				</animations>
			</entity>
			
			<entity name="target_2" rep="--" collideWithBall="false">
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
						<spawnEntity name="target_particles">
							<always />
						</spawnEntity>
					</spawnEntity>
				</collides>
				<animations>
					<animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true" />
				</animations>
			</entity>
		</entities>
	</templates>
</include>