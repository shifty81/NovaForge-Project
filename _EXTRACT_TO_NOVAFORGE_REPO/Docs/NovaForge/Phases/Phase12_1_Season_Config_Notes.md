# Phase 12.1 Season Config Notes

## Locked rule
- client can read/display season settings and local presentation preferences
- server is the normal authority for live seasonal timing and reset policy
- default season length should be around 180 days (~6 months)

## Why
This keeps the universe fresh over long-term play while still allowing deployment flexibility.

## Integration guidance
- client config should never override a connected server's live season authority
- local single-player or listen-server mode may use local server config
- dedicated servers should publish active season policy to clients
