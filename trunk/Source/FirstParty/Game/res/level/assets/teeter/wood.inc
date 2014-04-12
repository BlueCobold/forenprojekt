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
			<shape name="flipperteeter1" type="polygon">
				<vertex x="0" y="-15" />
				<vertex x="208" y="-15" />
				<vertex x="208" y="15" />
				<vertex x="0" y="15" />
			</shape>
			<shape name="flipperteeter2" type="polygon">
				<vertex x="0" y="-15" />
				<vertex x="-208" y="-15" />
				<vertex x="-208" y="15" />
				<vertex x="0" y="15" />
			</shape>
		</shapes>

		<physics>
			<physic name="teeter">
				<body type="kinematic" angle="0" />
				<fixture density="0.2" friction="0.8" restitution="0.4" />
			</physic>
			<physic name="target">
				<body type="kinematic" angle="0" />
				<fixture density="0.5" friction="0.8" restitution="0.1" />
			</physic>
			<physic name="verticalmoving1"> 
				<body type="kinematic" angle="0"> 
					<kinematics>
						<position axis="y">
							<setVar name="posy">
								<clamp>
									<sub>
										<var name="posy"/>
										<mouse axis="y" />
									</sub>
									<static value="-250" />
									<static value="250" />
								</clamp>
							
							</setVar>
						</position>
					</kinematics> 
				</body>  
				<fixture density="0.5" friction="0.8" restitution="0.1" /> 
			</physic>
		</physics>

		<entities>
			<entity name="PadellTeeter1" rep="P1" type="paddel">
				<physics name="verticalmoving1" shape="teeter2" />
				<sound name="bounce" />
				<collides>
					<changeGravity x="-10" y="0">
						<nop>
							<static value="1" />
							<setVar name="global:direction">
								<static value="1" />
							</setVar>
						</nop>
					</changeGravity>
				</collides>
				<animations>
					<animation texture="teeter_wooden_vertical" width="32" height="208" midx="16" midy="104" rotate="true" />
				</animations>
			</entity>
			<entity name="PadellTeeter2" rep="P2" type="paddel">
				<physics name="verticalmoving1" shape="teeter2" />
				<sound name="bounce" />
				<collides>
					<changeGravity x="10" y="0">
						<nop>
							<static value="1" />
							<setVar name="global:direction">
								<static value="1" />
							</setVar>
						</nop>
					</changeGravity>
				</collides>
				<animations>
					<animation texture="teeter_wooden_vertical" width="32" height="208" midx="16" midy="104" rotate="true" />
				</animations>
			</entity>
			<entity name="FlipperTeeter1" rep="F1" type="teeter">
				<physics name="teeter" shape="flipperteeter1" />
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden" width="208" height="32" midx="0" midy="16" rotate="true" alignment="vertical">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
												<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
									<inv>
										<static value="90"/>
									</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="FlipperTeeter2" rep="F2" type="teeter">
				<physics name="teeter" shape="flipperteeter2" />
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden" width="208" height="32" midx="208" midy="16" rotate="true" alignment="vertical">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
												<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
									<inv>
										<static value="90"/>
									</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="Teeter" rep="TT" type="teeter">
				<physics name="teeter" shape="teeter" />
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden" width="208" height="32" midx="104" midy="16" rotate="true" alignment="vertical">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
												<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
									<inv>
										<static value="90"/>
									</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="Teeter2" rep="T2" type="teeter">
				<physics name="teeter" shape="teeter2" />
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden_vertical" width="32" height="208" midx="16" midy="104" rotate="true">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
												<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
									<inv>
										<static value="90"/>
									</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>

			<entity name="Teeter3" rep="T3" type="teeter">
				<physics name="teeter" shape="teeter2" />
				<collides>
					<changeGravity x="-10" y="0">
						<nop>
							<static value="1" />
							<setVar name="global:direction">
								<static value="1" />
							</setVar>
					</nop>
					</changeGravity>
				</collides>
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden_vertical" width="32" height="208" midx="16" midy="104" rotate="true">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
													<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
										<inv>
											<static value="90"/>
										</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>
			<entity name="Teeter4" rep="T4" type="teeter">
				<physics name="teeter" shape="teeter2" />
				<collides>
					<changeGravity x="10" y="0">
						<nop>
							<static value="1" />
							<setVar name="global:direction">
								<static value="1" />
							</setVar>
					</nop>
					</changeGravity>
				</collides>
				<sound name="bounce" />
				<animations>
					<animation texture="teeter_wooden_vertical" width="32" height="208" midx="16" midy="104" rotate="true">
						<frameindex frames="7">
							<min>
								<mul>
									<add>
										<step>
											<sub>
												<angle />
													<static value="180" />
											</sub>
											<angle />
											<sub>
												<max>
													<angle />
													<static value="315" />
												</max>
												<static value="360" />
											</sub>
										</step>
										<static value="45" />
									</add>
										<inv>
											<static value="90"/>
										</inv>
									<static value="7"/>
								</mul>
								<static value="6"/>
							</min>
						</frameindex>
					</animation>
				</animations>
			</entity>
		</entities>
	</templates>
</include>