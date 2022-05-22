using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ManagedCodespace
{
  [StructLayout(LayoutKind.Sequential)]
  public struct NativeObject
  {
    public int a;
    public unsafe delegate* unmanaged[Cdecl]<int, void> callback;
  }

  public static class NativeInterface
  {
    /// <summary>
    /// Does basic math stuff.
    /// </summary>
    [NativeInvoked]
    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static unsafe int DoSimpleManagedThings(int a, int b)
    {
      return a + b;
    }

    [NativeInvoked]
    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static unsafe int DoManagedThings(NativeObject* obj, int b)
    {
      obj->callback(obj->a + b);
      return obj->a + b;
    }
  }
}

[AttributeUsage(AttributeTargets.Method)]
public class NativeInvokedAttribute : Attribute
{
}

[AttributeUsage((AttributeTargets.Method))]
public class NativeImplAttribute : Attribute
{
}

namespace Managed
{
  public static partial class Interop
  {
    [NativeImpl]
    public static unsafe partial int Add(int a, int b);

    [NativeImpl]
    public static unsafe partial void AddList(ReadOnlySpan<int> a, ReadOnlySpan<int> b, Span<int> o);
  }
}

namespace Managed
{
  [StructLayout(LayoutKind.Sequential)]
  public unsafe struct InteropSpan
  {
    private readonly void* _data;
    private readonly int _length;

    public InteropSpan(void* data, int length)
    {
      _data = data;
      _length = length;
    }

    public Span<T> ToSpan<T>()
    {
      return new Span<T>(_data, _length);
    }
  }

  public class InteropStub
  {
    public virtual int Add(int a, int b)
    {
      return default;
    }

    public virtual void AddSet(ReadOnlySpan<int> a, ReadOnlySpan<int> b, Span<int> o)
    {
    }
  }
  
  public static partial class Interop
  {
    public static InteropStub Stub { get; set; } = new();
    private static InteropImpl impl_ = new();

    [StructLayout(LayoutKind.Sequential)]
    public readonly unsafe struct InteropImpl
    {
      public InteropImpl(){}

      public readonly delegate* unmanaged[Cdecl]<int, int, int> Add = &AddMock;
      [UnmanagedCallersOnly(CallConvs = new[] {typeof(CallConvCdecl)})]
      private static int AddMock(int a, int b)
      {
        return Stub.Add(a, b);
      }

      public readonly delegate* unmanaged[Cdecl]<InteropSpan, InteropSpan, InteropSpan, void> AddSet = &AddSetMock;
      [UnmanagedCallersOnly(CallConvs = new[] {typeof(CallConvCdecl)})]
      private static void AddSetMock(InteropSpan a, InteropSpan b, InteropSpan o)
      {
        Stub.AddSet(a.ToSpan<int>(), b.ToSpan<int>(), o.ToSpan<int>());
      }
    }
    
    [NativeInvoked]
    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static unsafe void PopulateInterop(InteropImpl* impl)
    {
      impl_ = *impl;
    }

    public static unsafe partial int Add(int a, int b)
    {
      return impl_.Add(a, b);
    }

    public static unsafe partial void AddList(ReadOnlySpan<int> a, ReadOnlySpan<int> b, Span<int> o)
    {
      fixed (int* ap = a, bp = b, op = o)
      {
        var ai = new InteropSpan(ap, a.Length);
        var bi = new InteropSpan(bp, b.Length);
        var oi = new InteropSpan(op, o.Length);
        
        impl_.AddSet(ai, bi, oi);
      }
    }
  }
}
