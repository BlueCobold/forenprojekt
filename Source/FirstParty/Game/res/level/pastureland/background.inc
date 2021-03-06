<include>
	<templates>
		<functions>
			<function name="plantWobble">
				<!-- minAngle + ampBig*sin(time*perBig) + ampSmall*sin(time*perSmall) -->
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
	
	<objects>
		<background>

			<parallax width="1920" height="1080">
				<animation texture="pastureland_sky" width="1920" height="1080" rotate="false" />

				<animation texture="pastureland_clouds" width="126" height="53" srcx="352" srcy="68" y="250"><!-- tiny cloud (1) -->
					<constants speed="10" offset="400" width="2050" shift="-126" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>
				<animation texture="pastureland_clouds" width="138" height="68" srcx="352" srcy="0" y="225"><!-- tiny cloud (2) -->
					<constants speed="10" offset="1500" width="2070" shift="-140" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>
			</parallax>

			<parallax width="1955" height="1100">
				<animation texture="pastureland_back_mountain" x="0" y="460" width="1955" height="640" rotate="false">
					<stencil op="write" ref="2" mask="2"/>
				</animation>

				<animation texture="pastureland_clouds" width="200" srcx="246" srcy="263" height="87" y="190"><!-- mid cloud (3) -->
					<constants speed="12" offset="1165" width="2170" shift="-200" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>
				
				<animation spritesheet="pastureland_flowery" sprite="back_medium" x="370" y="625" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.5" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_medium_small_shadow" x="380" y="630" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="130" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_medium_small" x="380" y="630" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="130" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_small_shadow" x="380" y="635" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="130" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_small" x="380" y="635" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="130" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				
				<animation spritesheet="pastureland_flowery" sprite="back_medium_small" x="1845" y="555" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.6" perSmall="110" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_small_shadow" x="1855" y="558" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_small" x="1855" y="558" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_tiny_shadow" x="1850" y="561" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.2" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="back_tiny" x="1850" y="561" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.2" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
			</parallax>

			<parallax width="2125" height="1196">
				<animation spritesheet="pastureland_shadows" sprite="middle_shadow" x="93" y="435" blending="mul" rotate="false">
					<stencil op="test" ref="0" mask="3"/>
				</animation>
				<animation texture="pastureland_middle_mountain" x="228" y="571" width="1897" height="625" rotate="false">
					<stencil op="write" ref="2" mask="2"/>
				</animation>

				<animation texture="pastureland_clouds" srcx="246" srcy="138" width="240" height="125" y="150"><!-- big cloud (4) -->
					<constants speed="14.5" offset="2170" width="2380" shift="-250" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>

				<animation texture="pastureland_clouds" srcx="0" srcy="138" width="246" height="136" y="132"><!-- big cloud (5) -->
					<constants speed="14.5" offset="1100" width="2380" shift="-250" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>
				
				<animation spritesheet="pastureland_flowery" sprite="middle_large" x="1400" y="725" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.5" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_medium_large_shadow" x="1420" y="732" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="90" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_medium_large" x="1420" y="732" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.3" perSmall="90" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_medium_small_shadow" x="1423" y="737" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.6" perSmall="110" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_medium_small" x="1423" y="737" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.6" perSmall="110" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_small_shadow" x="1408" y="745" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="middle_small" x="1408" y="745" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
			</parallax>

			<parallax width="2310" height="1300">
				<animation spritesheet="pastureland_shadows" sprite="front_shadow" x="0" y="600" blending="mul" rotate="false">
					<stencil op="test" ref="0" mask="3"/>
				</animation>
				<animation texture="pastureland_front_mountain" x="0" y="733" width="1689" height="567" rotate="false">
					<stencil op="write" ref="2" mask="2"/>
				</animation>

				<animation texture="pastureland_clouds" width="352" height="138" y="120"><!-- huge cloud (6) -->
					<constants speed="17" offset="565" width="2640" shift="-352" />
					<!-- width - ((speed*time + offset) % width) + shift-->
					<position axis="x">
						<function name="cloudMovement" />
					</position>
				</animation>

				<animation spritesheet="pastureland_flowery" sprite="front_large" x="907" y="890" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.8" perSmall="130" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_medium_large_shadow" x="935" y="905" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="110" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_medium_large" x="935" y="905" blending="premul">
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.4" perSmall="110" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_medium_small_shadow" x="958" y="912" blending="mul">
					<stencil op="test" ref="0" mask="1"/>
				<!-- 02 mid flower   texture="pastureland_flowers_front2" width="149" height="187" midx="68" midy="180" -->
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.6" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_medium_small" x="958" y="912" blending="premul">
				<!-- 02 mid flower   texture="pastureland_flowers_front2" width="149" height="187" midx="68" midy="180" -->
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.6" perSmall="100" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_small_shadow" x="920" y="922" blending="mul"><!-- 01 front flower -->
					<stencil op="test" ref="0" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.2" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
				<animation spritesheet="pastureland_flowery" sprite="front_small" x="920" y="922" blending="premul"><!-- 01 front flower -->
					<stencil op="write" ref="1" mask="1"/>
					<constants minAngle="3" ampBig="6" perBig="50" ampSmall="1.2" perSmall="120" />
					<rotation>
						<function name="plantWobble" />
					</rotation>
				</animation>
			</parallax>
		</background>
	</objects>
</include>