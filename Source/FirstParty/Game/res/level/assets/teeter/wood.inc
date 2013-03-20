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
		
		<physics>
			<physic name="teeter">
				<body type="kinematic" angle="0" />
				<fixture density="0.2" friction="0.8" restitution="0.4" />
			</physic>
			<physic name="target">
				<body type="kinematic" angle="0" />
				<fixture density="0.5" friction="0.8" restitution="0.1" />
			</physic>
		</physics>
		
		<entities>
			<entity name="Teeter" rep="TT" type="teeter">
				<physics name="teeter" shape="teeter" />
				<sound name="test" />
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
				<sound name="test" />
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