`mix-to-nix` is a small tool for generating nix packages from `mix.lock` files.

## Usage

You can create an overlay, add `mix-to-nix` and use it to generate your nix
packages.

```nix
self: super:
{
  foobar =
    let
      src = fetchGit {
        url = https://github.com/example/foobar;
        rev = "examplerev000000000000000000000000000000";
        ref = "v1.0.0";
      };
    in
    self.callPackage (self.mixToNix src) {};

  mixToNix = self.callPackage (fetchGit {
    url = "https://github.com/serokell/mix-to-nix";
	rev = "examplerev000000000000000000000000000000";
	ref = "v1.0.0";
  }) {};
}
```

You can also use it to manually generate a `default.nix`:

```sh
mix deps.get
mix escript.build
./mix2nix /path/to/mix.lock /path/to/default.nix
```
