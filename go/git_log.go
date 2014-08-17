package main

import (
	"fmt"
	"os/exec"
)

func main() {
	cmd := exec.Command("git", "log")
	b, err := cmd.Output()
	if err != nil {
		fmt.Errorf("Failed to execute git log: %s - %s", err, string(b))
	}
	fmt.Println(string(b))
}
