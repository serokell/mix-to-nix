{
  outputs = { self }: {
    description = "Nix function that reads `mix.lock` file and builds a Mix project";

    overlay = final: prev: final.callPackage ./. { };
  };
}
