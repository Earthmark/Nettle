using System.Collections.Generic;
using System.Linq;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace InteropGenerator
{
  [Generator]
  public class CSharpSourceGenerator : ISourceGenerator
  {
    public void Initialize(GeneratorInitializationContext context)
    {
    }
    
    public void Execute(GeneratorExecutionContext context)
    {
      var methods = context.Compilation.SyntaxTrees
        .SelectMany(s => s.GetRoot().DescendantNodes())
        .Where(d => d.IsKind(SyntaxKind.MethodDeclaration))
        .OfType<MethodDeclarationSyntax>();

      foreach (var method in methods)
      {
        
      }
    }
  }
}