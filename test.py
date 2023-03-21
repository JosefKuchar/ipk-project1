#!/usr/bin/env python3
"""Tests for ipkcpc"""

import unittest
import subprocess
import argparse
import sys
import time

ADDR = '127.0.0.1'
TCP_PORT = '1234'
UDP_PORT = '1235'


def run(args, string=None):
    """Run ipkcpc with supplied arguments and input"""
    arguments = ' '.join(["./ipkcpc"] + args)
    pipe = subprocess.Popen(arguments, shell=True,
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    if string:
        pipe.stdin.write(string)
        pipe.stdin.flush()
        time.sleep(0.1)
        pipe.kill()
    # Wait for exit code
    out = pipe.wait()
    # Read output
    stdout = pipe.stdout.read()
    # Close all streams
    pipe.stdout.close()
    pipe.stdin.close()
    pipe.stderr.close()
    # Return exit code and output
    return (out, stdout)


class TestArgumentParsing(unittest.TestCase):
    """Argument parsing tests"""

    def test_no_arguments(self):
        """No arguments"""
        self.assertNotEqual(run([])[0], 0)

    def test_invalid_mode(self):
        """Invalid mode"""
        self.assertNotEqual(run(['-h', ADDR, '-p', TCP_PORT, '-m', 'a'])[0], 0)

    def test_invalid_host(self):
        """Invalid host"""
        self.assertNotEqual(
            run(['-h', 'foobar', '-p', TCP_PORT, '-m', 'tcp'])[0], 0)

    def test_invalid_port(self):
        """Invalid port"""
        self.assertNotEqual(run(['-h', ADDR, '-p', 'foo', '-m', 'tcp'])[0], 0)

    def test_missing_mode(self):
        """Missing mode"""
        self.assertNotEqual(run(['-h', ADDR, '-p', TCP_PORT,])[0], 0)

    def test_missing_host(self):
        """Missing host"""
        self.assertNotEqual(run(['-p', TCP_PORT, '-m', 'tcp'])[0], 0)

    def test_missing_port(self):
        """Missing port"""
        self.assertNotEqual(run(['-h', ADDR, '-m', 'tcp'])[0], 0)


class TestTCP(unittest.TestCase):
    """TCP tests"""

    def test_valid_expression(self):
        """Valid expression"""
        _, stdout = run(['-h', ADDR, '-p', UDP_PORT,
                         '-m', 'udp'], b'HELLO\nSOLVE (+ 1 2)\nBYE\n')
        self.assertEqual(stdout, b'HELLO\nRESULT 3\nBYE\n')

    def test_invalid_expression(self):
        """Invalid expression"""
        _, stdout = run(['-h', ADDR, '-p', UDP_PORT,
                         '-m', 'udp'], b'HELLO\nSOLVE (+ a b)\nBYE\n')
        self.assertEqual(stdout, b'HELLO\nBYE\n')


class TestUDP(unittest.TestCase):
    """UDP tests"""

    def test_valid_expression(self):
        """Valid expression"""
        _, stdout = run(['-h', ADDR, '-p', UDP_PORT,
                         '-m', 'udp'], b'(+ 1 2)\n')
        self.assertEqual(stdout, b'OK:3\n')

    def test_invalid_expression(self):
        """Invalid expression"""
        _, stdout = run(['-h', ADDR, '-p', UDP_PORT,
                         '-m', 'udp'], b'(+ 1 2)\n')
        self.assertEqual(stdout, b'ERR:Could not parse the message\n')


if __name__ == "__main__":
    # Setup argument parser
    parser = argparse.ArgumentParser(description="ipkcpc testing script")
    parser.add_argument('-a', help="Server address", required=True)
    parser.add_argument('-t', help="Server TCP port", required=True)
    parser.add_argument('-u', help="Server UDP port", required=True)
    # Parse known arguments
    parsed_args = parser.parse_known_args()
    ADDR = parsed_args[0].a
    TCP_PORT = parsed_args[0].t
    UDP_PORT = parsed_args[0].u
    # Pass rest args to unittest
    sys.argv[1:] = parsed_args[1]
    # Run tests
    unittest.main()
