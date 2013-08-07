<include>
	<templates>
		<shapes>
			<shape name="spike_okpart" type="complex_polygon">
				<polygon>
					<vertex x="0" y="59" />
					<vertex x="8" y="32" />
					<vertex x="16" y="32" />
					<vertex x="23" y="59" />
				</polygon>
				<polygon>
					<vertex x="24" y="59" />
					<vertex x="32" y="32" />
					<vertex x="40" y="32" />
					<vertex x="47" y="59" />
				</polygon>
				<polygon>
					<vertex x="48" y="59" />
					<vertex x="56" y="32" />
					<vertex x="64" y="32" />
					<vertex x="71" y="59" />
				</polygon>
				<polygon>
					<vertex x="72" y="59" />
					<vertex x="80" y="32" />
					<vertex x="88" y="32" />
					<vertex x="95" y="59" />
				</polygon>
				<polygon>
					<vertex x="96" y="59" />
					<vertex x="104" y="32" />
					<vertex x="112" y="32" />
					<vertex x="119" y="59" />
				</polygon>
			</shape>
			<shape name="spike_killer" type="complex_polygon">
				<polygon>
					<vertex x="-52" y="32" />
					<vertex x="-48" y="19" />
					<vertex x="-47" y="19" />
					<vertex x="-44" y="32" />
				</polygon>
				<polygon>
					<vertex x="-28" y="32" />
					<vertex x="-24" y="19" />
					<vertex x="-23" y="19" />
					<vertex x="-20" y="32" />
				</polygon>
				<polygon>
					<vertex x="-4" y="32" />
					<vertex x="0" y="19" />
					<vertex x="1" y="19" />
					<vertex x="4" y="32" />
				</polygon>
				<polygon>
					<vertex x="20" y="32" />
					<vertex x="24" y="19" />
					<vertex x="25" y="19" />
					<vertex x="28" y="32" />
				</polygon>
				<polygon>
					<vertex x="44" y="32" />
					<vertex x="48" y="19" />
					<vertex x="49" y="19" />
					<vertex x="52" y="32" />
				</polygon>
			</shape>
		</shapes>

		<physics>
			<physic name="spike_okpart">
				<body type="static" angle="0" />
				<fixture density="0.5" friction="0.8" restitution="0.1" />
			</physic>
		</physics>

		<entities>
			<entity name="spike_okpart" rep="^O" type="entity" collideWithBall="true">
				<physics name="spike_okpart" shape="spike_okpart" />
				<animations>
					<animation texture="gravity_arrow" y="20" srcx="224" srcy="128" width="24" height="40" />
					<animation texture="gravity_arrow" x="24" y="20" srcx="224" srcy="128" width="24" height="40" />
					<animation texture="gravity_arrow" x="48" y="20" srcx="224" srcy="128" width="24" height="40" />
					<animation texture="gravity_arrow" x="72" y="20" srcx="224" srcy="128" width="24" height="40" />
					<animation texture="gravity_arrow" x="96" y="20" srcx="224" srcy="128" width="24" height="40" />
				</animations>
			</entity>
			<entity name="spike_okpart" rep="^X" type="entity" collideWithBall="true">
				<physics name="spike_okpart" shape="spike_killer" />
				<onCollision>
					<killBall />
				</onCollision>
			</entity>
		</entities>
	</templates>
</include>