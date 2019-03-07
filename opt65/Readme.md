# Current work.

Evaluate Opt65's reliability. Debug has reported that some of its optimizations may be incorrect.

# Roadmap

## Format

Reformat the code for readability/maintainability. That code is way too compact at times to be readable.

## Bugfixes

These will be done with minimal changes to the original source code with the goal to produce a reliable version as quickly as possible so it can be integrated officially into the OSDK pipeline.

In this perspective, fixes will consist of either very small fixes or commenting out of buggy code.

## Feature enhancements

### Code rework

Evaluate if the code allows easy additions of new features. Rework it to be more data driven so that optimizations can be added independently of one another and can be easily individually activated/disabled.
This rework should be as limited as possible to allow enhancements without being too in-depth since the use of modern compilers in place of lcc65 is probably bound to become a much better alternative in the near future.

### New optimizations

#### Eliminate successive writes to same address

Currently does not detect successive writes to the same address separated by multiple instructions such as:
	    ldy #22
        lda (fp),y
        sta tmp0    <- writes tmp0
        lda #0
        sta tmp0+1
        lda #<(128)
        and tmp0
        sta tmp0    <- rewrites tmp0
Note: this might actually be a bug as this seems to be correctly handled in some other places.
Maybe opt65 does not sufficiently rework its own output (or at all)?

#### Remove void logical and arithmetical operations

List: ora #0, and #$FF, add #00, sbc #00.

