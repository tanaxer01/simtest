{ pkgs ? import <nixpkgs> { } }:

let
    kapack = import
        (fetchTarball "https://github.com/oar-team/nur-kapack/archive/master.tar.gz") {};
in

let my-packages = rec {
    my-batsim = kapack.batsim-410.overrideAttrs(old: rec {
        version = ""
        src = kapack.pkgs.fetchgit rec {
            url = "https://framagit.org/batsim/batsim.git";
            rev = version;
            sha256 = "";
        };
    });

    my-batsched = kapack.batshed-140.overrideAttrs(old: rec {
        version = ""
        src = kapack.pkgs.fetchgit rec {
            url = "https://framagit.org/batsim/batshed.git";
            rev = version;
            sha256 = "";
        };
    });

    my-pybatsim = kapack.batshed-140.overrideAttrs(old: rec {
        version = ""
        src = kapack.pkgs.fetchgit rec {
            url = "https://gitlab.inria.fr/batsim/pybatsim.git";
            rev = version;
            sha256 = "";
        };
    });
}



pkgs.mkShell {
  # nativeBuildInputs is usually what you want -- tools you need to run
  nativeBuildInputs = with pkgs; [
    nixpkgs-fmt
    rnix-lsp
    docker-client
    gnumake
    git
    rust
  ];

  buildInputs = with (customPackages pkgs kapack); [
    my-batsim
    my-batsched
    my-pybatsim
    kapack.batexpe
    kapack.pkgs.curl
  ];
}
