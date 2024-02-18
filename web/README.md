# Website

## General

Using [bun](https://bun.sh/) for development DX. Commands can be found in the `package.json`

in a terminal:
- cd into root of project
- `bun i`
- `bun run {cmd}`

## React + TypeScript + Vite

Vite template modified for multiple entry points and SSR. More to come once designs are made.

Intended to be uploaded to railway for 100% uptime, low cost and volume storage availability (see DB)

TODO: render caching in production mode

# API

## Types

- Shared types reside in `packages/constants/index.ts`
- API types reside in `src/api/types.ts`
- steam ids `BigInt`s by default. we're treating these as strings in the API so that non-steam users can be identified with a nanoid or uuid

## Security

Communication to-and-from the server secured with a simple shared secret used to sign a JWT. See this [Unreal plugin](https://www.unrealengine.com/marketplace/en-US/product/easy-jwt) for JWT operability.

The JWT should reside in the `Authorization` header as "Bearer ${JWT}"

## DB

The db is using `lowdb` and a local volume to persist a single json file. No server hosting, no latency. Brittle but quick for our use-case. We're not expecting breaking volumes of data and 