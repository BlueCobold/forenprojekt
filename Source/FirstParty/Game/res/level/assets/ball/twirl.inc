<include>
	<templates>
		<physics>
			<physic name="ball">
				<body type="dynamic" angle="0" angularDamping="4.0" />
				<fixture density="0.5" friction="0.7" restitution="0.4" />
			</physic>
		</physics>

		<entities>
		
			<entity name="ball_explosion" respawnable="true" stopWithLastAnimation="true" rep="--" collideWithBall="false">
				<animations>
					<animation copies="8" texture="sparkles" blending="add" width="32" height="32" midx="16" midy="16" rotate="true">
						<position axis="x">
							<mul>
								<time />
								<cache>
									<random min="-200" max="200" />
								</cache>
							</mul>
						</position>
						<position axis="y">
							<add>
								<mul>
									<cache>
										<random min="-300" max="100" />
									</cache>
									<time />
								</mul>
								<mul>
									<static value="200" />
									<time />
									<time />
								</mul>
							</add>
						</position>

						<color channel="alpha">
							<step>
								<sub>
									<static value="2" />
									<time />
								</sub>
								<stop />
								<static value="1" />
							</step>
						</color>

						<frameindex frames="8">
							<cache>
								<count start="0" increment="1" />
							</cache>
						</frameindex>
					</animation>
				</animations>
			</entity>

			<entity respawnable="true" name="ballspawn_particles" rep="--" collideWithBall="false">
				<animations>
					<animation copies="30" texture="sparkles" blending="add" width="32" height="32" midx="16" midy="16" rotate="true">
						<constants duration="0" size="1" time="0"/>
						<position axis="x">
							<step>
								<add>
									<time />
									<nop>
										<cache>
											<setVar name="duration">
												<cache>
													<random min="0.5" max="1.0" />
												</cache>
											</setVar>
										</cache>
										<setVar name="time">
											<clamp>
												<mul>
													<sub>
														<time />
														<sub>
															<static value="1" />
															<cache>
																<var name="duration" />
															</cache>
														</sub>
													</sub>
													<inv>
														<cache>
															<var name="duration" />
														</cache>
													</inv>
												</mul>
												<static value="0" />
												<static value="1" />
											</clamp>
										</setVar>
									</nop>
								</add>
								<static value="1" />
								<mul>
									<min>
										<pow>
											<sub>
												<static value="1" />
												<var name="time" />
											</sub>
											<static value="0.5" />
										</pow>
										<static value="1" />
									</min>
									<cache>
										<random min="-110" max="110" />
									</cache>
								</mul>
							</step>
						</position>
						<position axis="y">
							<step>
								<add>
									<time />
									<cache>
										<var name="duration" />
									</cache>
								</add>
								<static value="1" />
								<mul>
									<min>
										<pow>
											<sub>
												<static value="1" />
												<var name="time" />
											</sub>
											<static value="0.5" />
										</pow>
										<static value="1" />
									</min>
									<cache>
										<random min="-110" max="110" />
									</cache>
								</mul>
							</step>
						</position>
						<scale axis="x">
							<cache>
								<setVar name="size">
									<random min="0.3" max="1.0" />
								</setVar>
							</cache>
						</scale>
						<scale axis="y">
							<cache>
								<var name="size" />
							</cache>
						</scale>

						<color channel="red">
							<sub>
								<static value="1" />
								<mul>
									<cache>
										<random min="0.0" max="0.9" />
									</cache>
									<sub>
										<static value="1" />
										<var name="time" />
									</sub>
								</mul>
							</sub>
						</color>
						<color channel="green">
							<sub>
								<static value="1" />
								<mul>
									<cache>
										<random min="0.0" max="0.9" />
									</cache>
									<sub>
										<static value="1" />
										<var name="time" />
									</sub>
								</mul>
							</sub>
						</color>
						<color channel="blue">
							<sub>
								<static value="1" />
								<mul>
									<cache>
										<random min="0.0" max="0.9" />
									</cache>
									<sub>
										<static value="1" />
										<var name="time" />
									</sub>
								</mul>
							</sub>
						</color>
						<color channel="alpha">
							<step>
								<sub>
									<static value="1" />
									<var name="time" />
								</sub>
								<pow>
									<var name="time" />
									<static value="0.7" />
								</pow>
								<static value="0" />
							</step>
						</color>

						<frameindex frames="8">
							<step>
								<time />
								<cache>
									<int>
										<random min="0" max="8" />
									</int>
								</cache>
								<stop />
							</step>
						</frameindex>
					</animation>
				</animations>
			</entity>

			<!-- Ball -->
			<entity name="Ball" rep="BB" type="ball">
				<physics name="ball">
					<shape type="circle" radius="15" />
				</physics>
				<onRespawn>
					<spawnEntity name="ballspawn_particles" />
				</onRespawn>
				<onKill>
					<spawnEntity name="ball_explosion" />
				</onKill>
				<constants color="0" />
				<animations>
					<animation texture="ball_twirl" width="32" height="32" midx="16" midy="16" rotate="true">
						<frameindex frames="3">
							<var name="color" />
						</frameindex>
					</animation>
					<animation texture="ball_twirl" width="32" height="32" srcx="96" midx="16" midy="16" rotate="false">
						<frameindex frames="3">
							<var name="color" />
						</frameindex>
					</animation>
				</animations>
			</entity>
		</entities>
	</templates>
</include>