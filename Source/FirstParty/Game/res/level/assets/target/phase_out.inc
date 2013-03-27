<include>
	<templates>
		<entities>
			<entity name="target_1" rep="t1" type="target" collideWithBall="false">
				<constants color="0" />
				<physics name="target">
					<shape type="circle" radius="30" />
				</physics>
				<animations>
					<animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true">
						<frameindex frames="2">
							<setVar name="color">
								<mod>
									<mul>
										<time />
										<static value="0.5" />
									</mul>
									<static value="2" />
								</mod>
							</setVar>
						</frameindex>
					</animation>
				</animations>
				<collides>
					<spawnEntity name="target_2">
						<spawnEntity name="target_particles">
							<propertyFilter target="entity">
								<add>
									<static value="2" />
									<mul>
										<static value="-1" />
										<var name="color" />
									</mul>
								</add>
							</propertyFilter>
						</spawnEntity>
					</spawnEntity>
				</collides>
			</entity>
		</entities>
	</templates>
</include>