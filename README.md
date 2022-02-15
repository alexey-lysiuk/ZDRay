
# ZDRay baking utility for GZDoom

ZDRay is a node and lightmap generator for GZDoom. ZDRay is intended as a drop-in replacement for zdbsp, with the additional feature
that it can also bake lights. Once ZDRay has processed the level WAD it is ready to be used by GZDoom.

ZDRay is based on zdbsp for the node generation and originally used dlight for the lightmap generation. Special thanks to Randi Heit,
Samuel Villarreal, Christoph Oelckers and anyone else involved in creating or maintaining those tools.

The ray tracing code has been completely rewritten since. It now supports bounces and can do the ray tracing on the GPU. GPU ray
tracing requires a graphics card that has support for the Vulkan ray tracing API (for example, nvidia 10 series and higher). ZDRay
will automatically fall back to CPU tracing if no compatible GPU is found.

## ZDRay Usage

<pre>
Usage: zdray [options] sourcefile.wad
  -m, --map=MAP            Only affect the specified map
  -o, --output=FILE        Write output to FILE instead of tmp.wad
  -c, --comments           Write UDMF index comments
  -q, --no-prune           Keep unused sidedefs and sectors
  -N, --no-nodes           Do not rebuild nodes
  -g, --gl                 Build GL-friendly nodes
  -G, --gl-matching        Build GL-friendly nodes that match normal nodes
  -x, --gl-only            Only build GL-friendly nodes
  -5, --gl-v5              Create v5 GL-friendly nodes (overriden by -z and -X)
  -X, --extended           Create extended nodes (including GL nodes, if built)
  -z, --compress           Compress the nodes (including GL nodes, if built)
  -Z, --compress-normal    Compress normal nodes but not GL nodes
  -b, --empty-blockmap     Create an empty blockmap
  -r, --empty-reject       Create an empty reject table
  -R, --zero-reject        Create a reject table of all zeroes
  -E, --no-reject          Leave reject table untouched
  -p, --partition=NNN      Maximum segs to consider at each node (default 64)
  -s, --split-cost=NNN     Cost for splitting segs (default 8)
  -d, --diagonal-cost=NNN  Cost for avoiding diagonal splitters (default 16)
  -P, --no-polyobjs        Do not check for polyobject subsector splits
  -j, --threads=NNN        Number of threads used for raytracing (default 64)
  -S, --size=NNN           lightmap texture dimensions for width and height must
                           be in powers of two (1, 2, 4, 8, 16, etc)
  -C, --cpu-raytrace       Use the CPU for ray tracing
  -D, --vkdebug            Print messages from the vulkan validation layer
  -w, --warn               Show warning messages
  -t, --no-timing          Suppress timing information
  -V, --version            Display version information
      --help               Display this usage information
</pre>

## ZDRay UDMF properties

<pre>
thing // ZDRayInfo (ZDRay properties for the map)
{
	type = 9890;
	suncolor = &lt;int&gt; (color)
	sundirx = &lt;float&gt; (X direction for the sun)
	sundiry = &lt;float&gt; (Y direction for the sun)
	sundirz = &lt;float&gt; (Z direction for the sun)
	sampledistance = &lt;int&gt; (default: 8, map units each lightmap texel covers, must be in powers of two)
	bounces = &lt;int&gt; (default: 1, how many times light bounces off walls)
	gridsize = &lt;float&gt; (default: 32, grid density for the automatic light probes)
}

thing // Static point light
{
	type = 9876;
	lightcolor = &lt;int&gt; (color)
	lightintensity = &lt;float&gt; (default: 1)
	lightdistance = &lt;float&gt; (default: 0, no light)
}

thing // Static spotlight
{
	type = 9881;
	lightcolor = &lt;int&gt; (color)
	lightintensity = &lt;float&gt; (default: 1)
	lightdistance = &lt;float&gt; (default: 0, no light)
	lightinnerangle = &lt;float&gt; (default: 180)
	lightouterangle = &lt;float&gt; (default: 180)
}

thing // LightProbe (light sampling point for actors)
{
	type = 9875;
}

linedef // Line emissive surface
{
	lightcolor = &lt;int&gt; (color, default: white)
	lightintensity = &lt;float&gt; (default: 1)
	lightdistance = &lt;float&gt; (default: 0, no light)
}

sector // Sector plane emissive surface
{
	lightcolorfloor = &lt;int&gt; (color, default: white)
	lightintensityfloor = &lt;float&gt; (default: 1)
	lightdistancefloor = &lt;float&gt; (default: 0, no light)

	lightcolorceiling = &lt;int&gt; (color, default: white)
	lightintensityceiling = &lt;float&gt; (default: 1)
	lightdistanceceiling = &lt;float&gt; (default: 0, no light)
}
</pre>
