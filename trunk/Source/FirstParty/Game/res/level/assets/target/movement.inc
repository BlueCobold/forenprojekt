<include>
    <templates>
        <physics>
		<physic name="targetcircuit1"> 
			<body type="kinematic" angle="0"> 
				<kinematics> 
					<position axis="x"> 
						<mul> 
							<sine>
								<mul>
									<time />
									<static value="12" />
								</mul>
							</sine> 
							<static value="500" /> 
						</mul> 
					</position>
					<position axis="y"> 
						<mul> 
							<sine> 
								<add> 
									<static value="90" /> 
										<mul> 
											<time /> 
											<static value="12" /> 
										</mul> 
								</add> 
							</sine> 
							<static value="500" /> 
						</mul> 
					</position> 
				</kinematics> 
			</body> 
			<fixture density="0.5" friction="0.8" restitution="0.1" /> 
		</physic>

		<physic name="targetcircuit2"> 
			<body type="kinematic" angle="0"> 
				<kinematics> 
					<position axis="x"> 
						<mul> 
							<sine>
								<mul>
									<time />
									<static value="18" />
								</mul>
							</sine> 
							<static value="720" /> 
						</mul> 
					</position>
					<position axis="y"> 
						<mul> 
							<sine> 
								<add> 
									<static value="90" /> 
										<mul> 
											<time /> 
											<static value="18" /> 
										</mul> 
								</add> 
							</sine> 
							<static value="720" /> 
						</mul> 
					</position> 
				</kinematics> 
			</body> 
			<fixture density="0.5" friction="0.8" restitution="0.1" /> 
		</physic>

		<physic name="targetcircuit3"> 
			<body type="kinematic" angle="0"> 
				<kinematics> 
					<position axis="x"> 
						<mul> 
							<sine>
								<mul>
									<time />
									<static value="24" />
								</mul>
							</sine> 
							<static value="820" /> 
						</mul> 
					</position>
					<position axis="y"> 
						<mul> 
							<sine> 
								<add> 
									<static value="90" /> 
										<mul> 
											<time /> 
											<static value="24" /> 
										</mul> 
								</add> 
							</sine> 
							<static value="820" /> 
						</mul> 
					</position> 
				</kinematics> 
			</body> 
			<fixture density="0.5" friction="0.8" restitution="0.1" /> 
		</physic>

		<physic name="targetcircuit4"> 
			<body type="kinematic" angle="0"> 
				<kinematics> 
					<position axis="x"> 
						<mul> 
							<sine>
								<mul>
									<time />
									<static value="30" />
								</mul>
							</sine> 
							<static value="1020" /> 
						</mul> 
					</position>
					<position axis="y"> 
						<mul> 
							<sine> 
								<add> 
									<static value="90" /> 
										<mul> 
											<time /> 
											<static value="30" /> 
										</mul> 
								</add> 
							</sine> 
							<static value="1020" /> 
						</mul> 
					</position> 
				</kinematics> 
			</body> 
			<fixture density="0.5" friction="0.8" restitution="0.1" /> 
		</physic>


            <physic name="target2">
                <body type="kinematic" angle="0">
                    <kinematics>
                        <position axis="x">
                            <mul>
                                <sine>
                                    <mul>
                                        <time />
                                        <static value="100" />
                                    </mul>
                                </sine>
                                <static value="250" />
                            </mul>
                        </position>
                        <position axis="y">
                            <mul>
                                <sine>
                                    <mul>
                                        <time />
                                        <static value="120" />
                                    </mul>
                                </sine>
                                <static value="300" />
                            </mul>
                        </position>
                    </kinematics>
                </body>
                <fixture density="0.5" friction="0.8" restitution="0.1" />
            </physic>

	    <physic name="target">
                <body type="kinematic" angle="0">
                    <kinematics>
                        <position axis="x">
                            <mul>
                                <sine>
                                    <mul>
                                        <time />
                                        <static value="120" />
                                    </mul>
                                </sine>
                                <static value="30" />
                            </mul>
                        </position>
                        <position axis="y">
                            <mul>
                                <sine>
                                    <mul>
                                        <time />
                                        <static value="100" />
                                    </mul>
                                </sine>
                                <static value="30" />
                            </mul>
                        </position>
                    </kinematics>
                </body>
                <fixture density="0.5" friction="0.8" restitution="0.1" />
            </physic>
        </physics>
    </templates>
</include>