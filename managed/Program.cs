Console.WriteLine(typeof(Managed.Interop).AssemblyQualifiedName);
Console.WriteLine(nameof(Managed.Interop.PopulateInterop));
Console.WriteLine(typeof(ManagedCodespace.NativeInterface).AssemblyQualifiedName);

Console.WriteLine($"Unmanaged fallback {Managed.Interop.Add(1, 2)}");

Console.WriteLine($"Unmanaged 1 + 2 = {Managed.Interop.Add(1, 2)}");
int[] o = new int[4];
Managed.Interop.AddList(new[] { 1, 2, 3, 4 }, new[] { 2, 4, 6, 8 }, o);
Console.WriteLine($"Unmanaged [1, 2, 3, 4] + [2, 4, 6, 8] = [{string.Join(", ", o)}]");
