<include>
    <templates>
        <physics>
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