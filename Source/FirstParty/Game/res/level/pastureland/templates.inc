<include>
	<templates>
		<functions>
			<function name="plantWobble">
				<add>
					<var name="minAngle" />
					<mul>
						<sine>
							<mul>
								<time />
								<var name="perBig" />
							</mul>
						</sine>
						<var name="ampBig" />
					</mul>
					<mul>
						<sine>
							<mul>
								<time />
								<var name="perSmall" />
							</mul>
						</sine>
						<var name="ampSmall" />
					</mul>
				</add>
			</function>

			<function name="cloudMovement">
				<!-- width - ((speed*time + offset) % width) + shift-->
				<add>
					<var name="width" />
					<mul>
						<mod>
							<add>
								<mul>
									<time />
									<var name="speed" />
								</mul>
								<var name="offset" />
							</add>
							<var name="width" />
						</mod>
						<static value="-1" />
					</mul>
					<var name="shift" />
				</add>
			</function>
		</functions>
	</templates>
</include>