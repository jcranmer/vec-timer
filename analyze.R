#!/usr/bin/env Rscript
args <- commandArgs(TRUE)
version1 <- read.csv(args[1], header=FALSE)
version2 <- read.csv(args[2], header=FALSE)

nTrials <- 20
inc <- length(version1$V1) / nTrials
means1 <- seq(1, nTrials)
means2 <- seq(1, nTrials)
for (set in seq(inc, inc * nTrials, inc)) {
  means1[set / inc] <- mean(version1$V1[(set - inc):set])
  means2[set / inc] <- mean(version2$V1[(set - inc):set])
}
cat("Average time for", args[1], "was", mean(means1), "ns\n")
cat("Average time for", args[2], "was", mean(means2), "ns\n")
sig <- t.test(means1, means2)
if (mean(means1) < mean(means2)) {
  cat(args[1])
} else {
  cat(args[2])
}
cat(" was faster, p=", sig$p.value, "\n")
