{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-21.11.tar.gz") { } }:

let
  kapack = import (fetchTarball "https://github.com/oar-team/nur-kapack/archive/master.tar.gz") {};

  customPackages = pkgs: kapack: rec {
    my-batsim = kapack.batsim-410.overrideAttrs(old: rec {
      version = "3e7a7e5ecf1b749c306c532f828219e5a3d70862";
      src = kapack.pkgs.fetchgit rec {
        url = "https://framagit.org/batsim/batsim.git";
        rev = version;
        sha256 = "1f3xij13i4wgd667n0sqdkkzn5b11fz4bmxv9s0yivdniwn6md76";
      };
    });

    my-batsched = kapack.batsched-140.overrideAttrs(old: rec {
      version = "b020e48b89a675ae681eb5bcead01035405b571e";
      src = kapack.pkgs.fetchgit rec {
        url = "https://framagit.org/batsim/batsched.git";
        rev = version;
        sha256 = "1dmx2zk25y24z3m92bsfndvvgmdg4wy2iildjmwr3drmw15s75q0";
      };
    });

    my-pybatsim = kapack.pybatsim-320.overridePythonAttrs(old: rec {
      version = "fa6600eccd4e5ffbebfc7ecba05b64cf84c5e9d3";
      src = kapack.pkgs.fetchgit rec {
        url = "https://gitlab.inria.fr/batsim/pybatsim.git";
        rev = version;
        sha256 = "0nr8hqsx6y1k0lv82f4x3fjq6pz2fsd3h44cvnm7w21g4v3s6l6y";
      };
    });
  };
in

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    nixpkgs-fmt
    rnix-lsp
    docker-client
    gnumake
    git
    go
  ];

  buildInputs = with (customPackages pkgs kapack); [
    my-batsim
    my-batsched
    my-pybatsim
    kapack.batexpe
    kapack.pkgs.curl
  ];
}
