{
	inputs = {
		hyprland.url = "github:hyprwm/Hyprland";
	};

	outputs = {
		self,
		hyprland,
		...
	}: let
		inherit (hyprland.inputs) nixpkgs;
		lib = nixpkgs.lib;
		eachSystem = lib.genAttrs (import hyprland.inputs.systems);
		pkgsFor = eachSystem (system: import nixpkgs {localSystem = system;});
		hyprpm = (builtins.fromTOML (builtins.readFile ./hyprpm.toml));
	in {
		packages = eachSystem (system: let
			pkgs = pkgsFor.${system};
		in rec {
			bttr_dispatchers = pkgs.stdenv.mkDerivation {
				pname = "bttr_dispatchers";
				version = "git";
				src = ./.;
				nativeBuildInputs = with pkgs; [pkg-config meson ninja gcc14];
				buildInputs = with pkgs;
					[
						hyprland.packages.${system}.hyprland.dev
					]
					++ hyprland.packages.${system}.hyprland.buildInputs;

				meta = with pkgs.lib; {
					homepage = "https://github.com/${builtins.head hyprpm.repository.authors}/bttr_dispatchers";
					description = "${hyprpm.bttr_dispatchers.description}";
					platforms = platforms.linux;
				};
			};
		});

		devShells = eachSystem (system: let
			pkgs = pkgsFor.${system};
		in {
			default = pkgs.mkShell.override {stdenv = pkgs.gcc14Stdenv;} {
				shellHook = ''
					meson setup build --reconfigure
					cp ./build/compile_commands.json ./compile_commands.json
				'';
				name = "bttr_dispatchers";
				inputsFrom = [self.packages.${system}.bttr_dispatchers];
			};
		});
	};
}
